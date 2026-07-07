#include "Astrodata.hpp"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <unordered_map>

using json = nlohmann::json;

// Nome -> Código da API Horizons
const std::unordered_map<std::string, std::string> planetas = {
    {"Sol", "10"},
    {"Lua", "301"},
    {"Mercurio", "199"},
    {"Venus", "299"},
    {"Terra", "399"},
    {"Marte", "499"},
    {"Jupiter", "599"},
    {"Saturno", "699"},
    {"Urano", "799"},
    {"Netuno", "899"}
};

Astrodata::Astrodata()
    : Horinzontal(0), Vertical(0)
{
}

std::string Astrodata::obterTempoAtual(int minutosAdicionais)
{
    auto agora = std::chrono::system_clock::now();

    if (minutosAdicionais > 0)
        agora += std::chrono::minutes(minutosAdicionais);

    std::time_t tempo_t = std::chrono::system_clock::to_time_t(agora);
    std::tm* tempo_local = std::localtime(&tempo_t);

    std::stringstream ss;


    ss << "%27";
    ss << std::put_time(tempo_local, "%Y-%m-%d");
    ss << "%20";
    ss << std::put_time(tempo_local, "%H:%M");
    ss << "%27";

    return ss.str();
}

void Astrodata::obterDados(std::string planeta)
{
    auto it = planetas.find(planeta);

    if (it == planetas.end())
    {
        std::cerr << "Objeto \"" << planeta << "\" nao encontrado.\n";
        return;
    }

    std::string comando = it->second;
    std::string startTime = obterTempoAtual(0);
    std::string stopTime = obterTempoAtual(2);
    std::cout << "=====================\n";
    std::cout << "Planeta : " << planeta << '\n';
    std::cout << "Codigo  : " << comando << '\n';
    std::cout << "Inicio  : " << startTime << '\n';
    std::cout << "Fim     : " << stopTime << '\n';
    std::cout << "=====================\n";
    std::string url =
        "https://ssd.jpl.nasa.gov/api/horizons.api?"
        "format=json"
        "&COMMAND=%27" + comando + "%27"
        "&EPHEM_TYPE=OBSERVER"
        "&CENTER=%27coord@399%27"
        "&SITE_COORD=%27-46.6248,-23.7257,0.74%27"
        "&START_TIME="+startTime+
        "&STOP_TIME="+stopTime+
        "&STEP_SIZE=%271%20m%27"
        "&QUANTITIES=%274%27";

   
   
    cpr::Response r = cpr::Get(cpr::Url{url});
    if (r.status_code != 200)
    {
         
        std::cout << r.url.str() << '\n';
        std::cerr << "Falha na requisicao HTTP. Status: "
                  << r.status_code << '\n';
        return;
    }

    try
    {
        json dados = json::parse(r.text);
       
        if (!dados.contains("result"))
        {
            std::cerr << "Resposta da API invalida.\n";
            return;
        }

        std::string resultado_bruto = dados["result"];

        size_t inicio = resultado_bruto.find("$$SOE");
        size_t fim = resultado_bruto.find("$$EOE");

        if (inicio == std::string::npos || fim == std::string::npos)
        {
            std::cerr << "Nao foi possivel localizar os dados.\n";
            return;
        }

        std::string bloco_dados =
            resultado_bruto.substr(inicio + 5, fim - (inicio + 5));

        std::stringstream ss(bloco_dados);

        std::string data;
        std::string hora;
        char indicador;
        

        if (ss >> data >> hora >> indicador >> Horinzontal >> Vertical)
        {
            std::cout << "\nObjeto: " << planeta << '\n';
            std::cout << "Azimute : " << Horinzontal << " graus\n";
            std::cout << "Altitude: " << Vertical << " graus\n";
        }
        else
        {
            std::cerr << "Erro ao interpretar os dados retornados.\n";
        }
    }
    catch (const json::parse_error& e)
    {
        std::cerr << "Erro ao decodificar JSON: "
                  << e.what() << '\n';
    }
}

float Astrodata::obterHorinzontal()
{
    return this->Horinzontal;
}

float Astrodata::obterVertical()
{
    return this->Vertical;
}