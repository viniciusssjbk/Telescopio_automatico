#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <optional>

class Application
{
private:
    sf::RenderWindow janela;

    sf::Texture textura;
    sf::Font fonte;

    sf::RectangleShape fundo;
    sf::RectangleShape barraSuperior;
    sf::RectangleShape menuLateral;
    sf::RectangleShape areaPrincipal;
    sf::RectangleShape barraInferior;

    sf::RectangleShape linhaSuperior;
    sf::RectangleShape linhaInferior;
    sf::RectangleShape linhaMenu;

    sf::RectangleShape caixaSelecao;

    std::optional<sf::Text> titulo;
    std::optional<sf::Text> telaAtual;
    std::optional<sf::Text> menuTitulo;
    std::optional<sf::Text> status;
    std::vector<sf::Text> itensMenu;
    std::vector<sf::FloatRect> hitboxesMenu;

    // ===================== RADAR MODERNO =====================
    sf::VertexArray estrelasFundo;              // fundo estrelado

    std::vector<sf::CircleShape> circulosRadar; // anéis concêntricos
    std::vector<sf::VertexArray> linhasRadar;   // linhas radiais pontilhadas

    sf::RectangleShape linhaHorizontal;         // eixo horizontal da mira
    sf::RectangleShape linhaVertical;           // eixo vertical da mira

    sf::CircleShape centroExterno;              // anel externo da mira
    sf::CircleShape centroInterno;              // ponto central da mira
    sf::CircleShape haloCentral;                // brilho pulsante (estático)

    // marcadores cardeais – agora std::optional para evitar default‑construction
    std::optional<sf::Text> marcadorN, marcadorS, marcadorL, marcadorO;
    std::optional<sf::Text> marcadorNE, marcadorNO, marcadorSE, marcadorSO;

    // ===================== PLANETA E RASTREAMENTO =====================
    sf::CircleShape planeta;
    sf::CircleShape brilhoPlaneta;
    sf::CircleShape destaquePlaneta;            // ponto de luz no planeta
    sf::VertexArray linhaRastreamento;          // linha do centro ao planeta
    sf::VertexArray rastroLaser;                // linha dupla para efeito glow
    sf::VertexArray arcoOrbital;                // arco ao redor do planeta

    // ===================== HUD DE COORDENADAS =====================
    std::optional<sf::Text> infoAzimute;
    std::optional<sf::Text> infoAltitude;

    // ===================== VARIÁVEIS DE POSIÇÃO =====================
    sf::Vector2f posicaoAlvo;
    sf::Vector2f destinoAlvo;

    std::string modoTeles;
    float motorH = 0;
    float motorV = 0;
    float Altitude = 0;
    float Longitude = 0;

public:
    Application();
    void executar();

private:
    void carregarRecursos();
    void criarInterface();
    void criarFundo();
    void criarBarraSuperior();
    void criarMenuLateral();
    void criarAreaPrincipal();
    void criarBarraInferior();
    void criarTextos();
    void atualizarPosicaoPlaneta();
    void atualizar();
    void desenhar();
};