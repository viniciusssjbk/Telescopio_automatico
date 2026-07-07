#include "application.hpp"
#include "Telescopio.hpp"
#include <iostream>
#include <cmath>
#include <vector>
constexpr float PI = 3.14159265358979323846f;

Application::Application()
    : janela(sf::VideoMode({1280, 720}), "Telescopio Automatico"),
    motorH(0),motorV(0),Altitude(0),Longitude(0),modoTeles("Automatico")
{
    janela.setFramerateLimit(60);
    carregarRecursos();
    criarInterface();
    posicaoAlvo = {770.f,380.f};
    destinoAlvo = {770.f,380.f};
}

void Application::carregarRecursos()
{
    if (!textura.loadFromFile("../assets/images/fundo.png"))
        std::cerr << "Erro ao carregar fundo.png\n";

    if (!fonte.openFromFile("../assets/fonts/Orbitron-SemiBold.ttf"))
        std::cerr << "Erro ao carregar Orbitron.\n";
}

void Application::criarInterface()
{
    criarFundo();
    criarBarraSuperior();
    criarMenuLateral();
    criarAreaPrincipal();
    criarBarraInferior();
    criarTextos();
}

void Application::criarFundo()
{
    fundo.setSize({1280.f, 720.f});
    fundo.setPosition({0.f, 0.f});
    fundo.setTexture(&textura);
}

void Application::criarBarraSuperior()
{
    barraSuperior.setSize({1280.f, 70.f});
    barraSuperior.setPosition({0.f, 0.f});
    barraSuperior.setFillColor(sf::Color(12, 16, 30, 240));

    linhaSuperior.setSize({1280.f, 2.f});
    linhaSuperior.setPosition({0.f, 70.f});
    linhaSuperior.setFillColor(sf::Color(120, 80, 255));
}

void Application::criarMenuLateral()
{
    menuLateral.setSize({260.f, 610.f});
    menuLateral.setPosition({0.f, 70.f});
    menuLateral.setFillColor(sf::Color(16, 20, 38, 235));

    linhaMenu.setSize({2.f, 610.f});
    linhaMenu.setPosition({260.f, 70.f});
    linhaMenu.setFillColor(sf::Color(80, 100, 200));
}

void Application::criarAreaPrincipal()
{
    // ================ FUNDO DO PAINEL ================
    areaPrincipal.setSize({1020.f,610.f});
    areaPrincipal.setPosition({260.f,70.f});
    areaPrincipal.setFillColor(sf::Color(5,10,20,230));

    caixaSelecao.setSize({230.f,40.f});
    caixaSelecao.setPosition({1015.f,15.f});
    caixaSelecao.setFillColor(sf::Color(20,25,45,220));
    caixaSelecao.setOutlineThickness(1.f);
    caixaSelecao.setOutlineColor(sf::Color(80,180,255));

    // ================ ESTRELAS DE FUNDO ================
    estrelasFundo.setPrimitiveType(sf::PrimitiveType::Points);
    estrelasFundo.clear();
    // Gera 400 estrelas aleatórias dentro da área principal
    for (int i = 0; i < 400; ++i)
    {
        float x = 260.f + rand() % 1020;
        float y = 70.f + rand() % 610;
        float brilho = 50 + rand() % 155;  // opacidade variada
        sf::Color cor(200, 220, 255, brilho);
        estrelasFundo.append(sf::Vertex(sf::Vector2f(x, y), cor));
    }

    // ================ RADAR: CENTRO ================
    sf::Vector2f centro(770.f,380.f);

    // Anéis concêntricos (estilo neon)
    circulosRadar.clear();
    for(float raio : {220.f,170.f,120.f,70.f})
    {
        sf::CircleShape circulo;
        circulo.setRadius(raio);
        circulo.setOrigin(sf::Vector2f(raio,raio));
        circulo.setPosition(centro);
        circulo.setFillColor(sf::Color::Transparent);
        circulo.setOutlineThickness(1.2f);
        circulo.setOutlineColor(sf::Color(70, 180, 255, 120));
        circulosRadar.push_back(circulo);
    }

    // Linhas radiais pontilhadas (a cada 45°)
    linhasRadar.clear();
    for(int grau=0; grau<360; grau+=45)
    {
        float angulo = grau * PI / 180.f;
        sf::VertexArray linha(sf::PrimitiveType::Lines, 20); // 10 segmentos (tracejado)
        float raioMax = 220.f;
        for (int i = 0; i < 10; ++i)
        {
            float r1 = i * (raioMax / 10.f);
            float r2 = r1 + (raioMax / 20.f);
            if (r2 > raioMax) r2 = raioMax;
            linha[i*2].position = sf::Vector2f(centro.x + cos(angulo)*r1, centro.y + sin(angulo)*r1);
            linha[i*2+1].position = sf::Vector2f(centro.x + cos(angulo)*r2, centro.y + sin(angulo)*r2);
            linha[i*2].color = sf::Color(70, 180, 255, 80);
            linha[i*2+1].color = sf::Color(70, 180, 255, 80);
        }
        linhasRadar.push_back(linha);
    }

    // Eixos da mira (horizontal / vertical)
    linhaHorizontal.setSize({440.f, 1.5f});
    linhaHorizontal.setPosition({550.f, 380.f});
    linhaHorizontal.setFillColor(sf::Color(70, 200, 255, 150));

    linhaVertical.setSize({1.5f, 440.f});
    linhaVertical.setPosition({770.f, 160.f});
    linhaVertical.setFillColor(sf::Color(70, 200, 255, 150));

    // Marcadores cardeais (agora usando std::optional)
    auto criarMarcador = [&](std::optional<sf::Text>& opt, const std::string& texto, float angulo, float distancia) {
        opt.emplace(fonte, texto, 14);
        opt->setFillColor(sf::Color(180, 220, 255, 200));
        opt->setPosition(sf::Vector2f(
            centro.x + cos(angulo) * distancia - opt->getGlobalBounds().size.x/2.f,
            centro.y + sin(angulo) * distancia - opt->getGlobalBounds().size.y/2.f
        ));
    };
    criarMarcador(marcadorN, "N", -90.f * PI/180.f, 240.f);
    criarMarcador(marcadorS, "S",  90.f * PI/180.f, 240.f);
    criarMarcador(marcadorL, "L",   0.f * PI/180.f, 240.f);
    criarMarcador(marcadorO, "O", 180.f * PI/180.f, 240.f);
    criarMarcador(marcadorNE, "NE", -45.f * PI/180.f, 240.f);
    criarMarcador(marcadorNO, "NO", -135.f * PI/180.f, 240.f);
    criarMarcador(marcadorSE, "SE",  45.f * PI/180.f, 240.f);
    criarMarcador(marcadorSO, "SO", 135.f * PI/180.f, 240.f);

    // Ícone central do telescópio (mira estilizada)
    centroExterno.setRadius(12.f);
    centroExterno.setOrigin(sf::Vector2f(12.f,12.f));
    centroExterno.setPosition(centro);
    centroExterno.setFillColor(sf::Color::Transparent);
    centroExterno.setOutlineThickness(2.f);
    centroExterno.setOutlineColor(sf::Color(0, 255, 200, 200));

    centroInterno.setRadius(4.f);
    centroInterno.setOrigin(sf::Vector2f(4.f,4.f));
    centroInterno.setPosition(centro);
    centroInterno.setFillColor(sf::Color(0, 255, 200));

    haloCentral.setRadius(20.f);
    haloCentral.setOrigin(sf::Vector2f(20.f,20.f));
    haloCentral.setPosition(centro);
    haloCentral.setFillColor(sf::Color(0, 255, 200, 20));

    // ================ PLANETA E DESTAQUE ================
    brilhoPlaneta.setRadius(22.f);
    brilhoPlaneta.setOrigin(sf::Vector2f(22.f,22.f));
    brilhoPlaneta.setFillColor(sf::Color(255, 140, 0, 40));

    planeta.setRadius(8.f);
    planeta.setOrigin(sf::Vector2f(8.f,8.f));
    planeta.setFillColor(sf::Color(255, 200, 50));
    planeta.setPosition(centro);  // posição inicial

    destaquePlaneta.setRadius(2.5f);
    destaquePlaneta.setOrigin(sf::Vector2f(2.5f,2.5f));
    destaquePlaneta.setFillColor(sf::Color(255, 255, 255, 180));

    // Linha de rastreamento (laser)
    linhaRastreamento = sf::VertexArray(sf::PrimitiveType::Lines, 2);
    linhaRastreamento[0].color = sf::Color(0, 255, 255, 200);
    linhaRastreamento[1].color = sf::Color(255, 180, 50, 200);

    rastroLaser = sf::VertexArray(sf::PrimitiveType::Lines, 2);
    rastroLaser[0].color = sf::Color(0, 255, 255, 50);
    rastroLaser[1].color = sf::Color(255, 180, 50, 50);

    // Arco orbital (será redesenhado no desenhar)
    arcoOrbital = sf::VertexArray(sf::PrimitiveType::LineStrip, 32);

    // ================ HUD DE COORDENADAS ================
    infoAzimute.emplace(fonte, "AZ: 0.0°", 16);
    infoAzimute->setFillColor(sf::Color(100, 220, 255, 230));
    infoAzimute->setPosition({272.f, 82.f});

    infoAltitude.emplace(fonte, "ALT: 0.0°", 16);
    infoAltitude->setFillColor(sf::Color(100, 220, 255, 230));
    infoAltitude->setPosition({272.f, 105.f});
}

void Application::criarBarraInferior()
{
    barraInferior.setSize({1280.f, 40.f});
    barraInferior.setPosition({0.f, 680.f});
    barraInferior.setFillColor(sf::Color(12, 16, 30, 240));

    linhaInferior.setSize({1280.f, 2.f});
    linhaInferior.setPosition({0.f, 678.f});
    linhaInferior.setFillColor(sf::Color(120, 80, 255));
}

void Application::atualizarPosicaoPlaneta()
{
    float azimute = Longitude;
    float altitude = Altitude;
    float angulo = (azimute-90.f) * PI / 180.f;
    float distancia = (90.f-altitude) / 90.f * 200.f;

    destinoAlvo = sf::Vector2f(
        770.f + cos(angulo) * distancia,
        380.f + sin(angulo) * distancia
    );
}

void Application::criarTextos()
{
    titulo.emplace(fonte, "TELESCOPIO AUTOMATICO", 28);
    titulo->setPosition({20.f, 15.f});
    titulo->setFillColor(sf::Color(233, 243, 255));

    telaAtual.emplace(fonte, "Tela: Lua", 18);
    telaAtual->setPosition({1020.f, 25.f});
    telaAtual->setFillColor(sf::Color(180, 200, 255));

    menuTitulo.emplace(fonte, "MENU", 30);
    menuTitulo->setPosition({50.f, 90.f});
    menuTitulo->setFillColor(sf::Color(180, 200, 255));

    std::vector<std::string> nomesItens = {
        "Lua", "Mercurio", "Venus", "Marte",
        "Jupiter", "Saturno", "Urano", "Netuno",
        "Manual", "Configuracoes"
    };

    float y = 150.f;
    for (const auto& nome : nomesItens)
    {
        sf::Text item(fonte, nome, 20);
        item.setPosition({30.f, y});
        item.setFillColor(sf::Color(233, 243, 255));
        itensMenu.push_back(item);

        sf::FloatRect bounds = item.getGlobalBounds();
        sf::FloatRect hitbox(
            bounds.position - sf::Vector2f(5.f, 3.f),
            bounds.size + sf::Vector2f(10.f, 6.f)
        );
        hitboxesMenu.push_back(hitbox);

        y += 35.f;
    }

    std::string textoStatus = 
        "MOTOR H: " + std::to_string(motorH) +
        " | MOTOR V: " + std::to_string(motorV) +
        " | ALT: " + std::to_string(Altitude) +
        " | LONG: " + std::to_string(Longitude) +
        " | ONLINE";

    status.emplace(fonte, textoStatus, 16);
    status->setPosition({20.f, 688.f});
    status->setFillColor(sf::Color(233, 243, 255));

    // ATUALIZAÇÃO DA POSIÇÃO DO PLANETA (mantida conforme original)
    posicaoAlvo.x +=(destinoAlvo.x-posicaoAlvo.x)*0.05f;
    posicaoAlvo.y +=(destinoAlvo.y-posicaoAlvo.y)*0.05f;
    planeta.setPosition(posicaoAlvo);
    brilhoPlaneta.setPosition(posicaoAlvo);
    destaquePlaneta.setPosition(posicaoAlvo + sf::Vector2f(-3.f, -3.f)); // reflexo
    linhaRastreamento[0].position = sf::Vector2f(770.f,380.f);
    linhaRastreamento[1].position = posicaoAlvo;
    rastroLaser[0].position = sf::Vector2f(770.f,380.f);
    rastroLaser[1].position = posicaoAlvo;
}

void Application::atualizar()
{
    while (const std::optional evento = janela.pollEvent())
    {
        if (evento->is<sf::Event::Closed>())
            janela.close();

        else if (const auto* clique = evento->getIf<sf::Event::MouseButtonPressed>())
        {
            if (clique->button == sf::Mouse::Button::Left)
            {
                sf::Vector2f posMouse(clique->position.x, clique->position.y);
                for (size_t i = 0; i < itensMenu.size(); ++i)
                {
                    if (hitboxesMenu[i].contains(posMouse))
                    {
                        std::string nome = itensMenu[i].getString().toAnsiString();
                        telaAtual->setString("Tela: " + nome);
                        if(nome=="Configuracoes"){
                            // vazio proposital
                        }
                        else if(nome=="Manual"){
                            modoTeles = "Manual";
                            status->setString(
                                "MOTOR H: " + std::to_string(motorH) +
                                "   | MOTOR V: " + std::to_string(motorV) +
                                "   | ALT: " + std::to_string(Altitude) +
                                "   | LONG: " + std::to_string(Longitude) +
                                "   | MODO: "+ modoTeles
                            );
                        }
                        else{
                            modoTeles = "Automatico";
                            Telescopio dados;
                            dados.Mover_Telescopio(nome);
                            this->Altitude = dados.obterAnguloVertical();
                            this->Longitude = dados.obterAnguloHorizontal();
                            atualizarPosicaoPlaneta();

                            // Recalcula posição do planeta imediatamente
                            posicaoAlvo = destinoAlvo; // salto direto (já que a animação é chamada só uma vez)
                            planeta.setPosition(posicaoAlvo);
                            brilhoPlaneta.setPosition(posicaoAlvo);
                            destaquePlaneta.setPosition(posicaoAlvo + sf::Vector2f(-3.f, -3.f));
                            linhaRastreamento[0].position = sf::Vector2f(770.f,380.f);
                            linhaRastreamento[1].position = posicaoAlvo;
                            rastroLaser[0].position = sf::Vector2f(770.f,380.f);
                            rastroLaser[1].position = posicaoAlvo;

                            status->setString(
                                "MOTOR H: " + std::to_string(motorH) +
                                "   | MOTOR V: " + std::to_string(motorV) +
                                "   | ALT: " + std::to_string(Altitude) +
                                "   | LONG: " + std::to_string(Longitude) +
                                "   | MODO: "+ modoTeles
                            );
                        }
                        break;
                    }
                }
            }
        }
        else if (const auto* mover = evento->getIf<sf::Event::MouseMoved>())
        {
            sf::Vector2f posMouse(mover->position.x, mover->position.y);

            for (size_t i = 0; i < itensMenu.size(); ++i)
                itensMenu[i].setFillColor(sf::Color(233, 243, 255));

            for (size_t i = 0; i < itensMenu.size(); ++i)
            {
                if (hitboxesMenu[i].contains(posMouse))
                {
                    itensMenu[i].setFillColor(sf::Color(255, 230, 100));
                    break;
                }
            }
        }
    }
}

void Application::desenhar()
{
    janela.clear(sf::Color(4, 6, 11));

    // Fundo principal e interface fixa
    janela.draw(fundo);
    janela.draw(barraSuperior);
    janela.draw(menuLateral);
    janela.draw(areaPrincipal);

    // ========== NOVA ÁREA PRINCIPAL (ordem de camadas) ==========
    // 1. Fundo estrelado
    janela.draw(estrelasFundo);

    // 2. Anéis e linhas do radar
    for(auto& c : circulosRadar) janela.draw(c);
    for(auto& l : linhasRadar)   janela.draw(l);

    // 3. Marcadores cardeais (dereferenciando os optionals)
    janela.draw(*marcadorN);
    janela.draw(*marcadorS);
    janela.draw(*marcadorL);
    janela.draw(*marcadorO);
    janela.draw(*marcadorNE);
    janela.draw(*marcadorNO);
    janela.draw(*marcadorSE);
    janela.draw(*marcadorSO);

    // 4. Eixos da mira
    janela.draw(linhaHorizontal);
    janela.draw(linhaVertical);

    // 5. Rastro laser (glow fraco por baixo)
    janela.draw(rastroLaser);

    // 6. Linha de rastreamento principal
    janela.draw(linhaRastreamento);

    // 7. Brilho do planeta
    janela.draw(brilhoPlaneta);

    // 8. Arco orbital ao redor do planeta
    {
        sf::Vector2f pos = planeta.getPosition();
        float raioArco = 14.f;
        float inicio = -30.f * PI / 180.f;
        float fim    = 210.f * PI / 180.f;
        int pontos = 32;
        arcoOrbital.clear();
        arcoOrbital.setPrimitiveType(sf::PrimitiveType::LineStrip);
        for (int i = 0; i < pontos; ++i)
        {
            float ang = inicio + (fim - inicio) * i / (pontos-1);
            float px = pos.x + cos(ang) * raioArco;
            float py = pos.y + sin(ang) * raioArco;
            arcoOrbital.append(sf::Vertex(sf::Vector2f(px, py), sf::Color(100, 200, 255, 120)));
        }
        janela.draw(arcoOrbital);
    }

    // 9. Planeta e destaque
    janela.draw(planeta);
    janela.draw(destaquePlaneta);

    // 10. Ícone central do telescópio
    janela.draw(haloCentral);
    janela.draw(centroExterno);
    janela.draw(centroInterno);

    // 11. HUD de coordenadas
    infoAzimute->setString("AZ: " + std::to_string(Longitude).substr(0,5) + (char)248);
    infoAltitude->setString("ALT: " + std::to_string(Altitude).substr(0,5) + (char)248);
    janela.draw(*infoAzimute);
    janela.draw(*infoAltitude);

    // ========== DEMAIS ELEMENTOS DA UI ==========
    janela.draw(barraInferior);
    janela.draw(linhaSuperior);
    janela.draw(linhaInferior);
    janela.draw(linhaMenu);
    janela.draw(caixaSelecao);

    janela.draw(*titulo);
    janela.draw(*telaAtual);
    janela.draw(*menuTitulo);

    for (auto& item : itensMenu)
        janela.draw(item);

    janela.draw(*status);

    janela.display();
}

void Application::executar()
{
    while (janela.isOpen())
    {
        atualizar();
        desenhar();
    }
}