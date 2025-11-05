#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

#define TERRITORY_AMOUNT 5
#define MISSION_AMOUNT 2
#define MAX_INPUT_BUFFER 100 // Buffer para leitura segura

// Estruturas de dados para o WAR
typedef enum {
    MISSION_DESTROY_ARMY = 1,
    MISSION_CONQUER_TERRITORIES = 2
} MissionType;

// Struct Territorio, o coração do jogo
typedef struct {
    char name[30];       // Nome do território
    char army_color[20]; // Cor do exército que domina
    int troop_count;     // Quantidade de tropas
} Territory;

// Struct para gerenciar a Missão do Nível Mestre
typedef struct {
    MissionType type;
    char target_color[20]; // Cor a ser destruída
    int territories_needed; // Quantidade a ser conquistada
} Mission;


/********************************************
 * FUNÇÕES DE UTILIDADE E INPUT SEGURO      *
 *******************************************/

// Função para limpar o buffer (sempre esqueço disso)
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Rola o dado (random!)
int roll_dice() {
    return (rand() % 6) + 1;
}

// Tenta ler um inteiro, tratando lixo de texto
int read_int_safe() {
    char buffer[MAX_INPUT_BUFFER];
    int value;

    // Looping para garantir que a entrada seja numérica
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        // Tenta ler o inteiro e garante que não há lixo de texto
        char extra;
        // Se sscanf retornar 1, lemos o número. Se retornar 2, tinha lixo.
        if (sscanf(buffer, "%d %c", &value, &extra) == 1 ||
            (sscanf(buffer, "%d", &value) == 1 && buffer[strlen(buffer) - 1] == '\n')) {

            if (value >= 0) { // Garante que a tropa/ID é positiva (ou zero para sair)
                return value;
            }
        }

        printf("Entrada inválida. Digite um número inteiro (>= 0): ");
    }
    return -1; // Falha (EOF)
}

// Escolhe a missão aleatoriamente
void initialize_mission(Mission *mission, const Territory *map, int amount) {
    mission->type = (rand() % MISSION_AMOUNT) + 1;

    if (mission->type == MISSION_DESTROY_ARMY) {
        // Exemplo: sorteia uma cor comum para destruir
        const char *colors[] = {"VERDE", "AZUL", "AMARELO", "ROXO"};
        int random_index = rand() % 4;
        strcpy(mission->target_color, colors[random_index]);
    } else if (mission->type == MISSION_CONQUER_TERRITORIES) {
        mission->territories_needed = 3; // Missão de conquistar 3
    }
}


/********************************************
 * FUNÇÕES DE UTILIDADE E INPUT SEGURO      *
 *******************************************/

// Função para limpar o buffer (sempre esqueço disso)
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Rola o dado (random!)
int roll_dice() {
    return (rand() % 6) + 1;
}

// Tenta ler um inteiro, tratando lixo de texto
int read_int_safe() {
    char buffer[MAX_INPUT_BUFFER];
    int value;

    // Looping para garantir que a entrada seja numérica
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        // Tenta ler o inteiro e garante que não há lixo de texto
        char extra;
        // Se sscanf retornar 1, lemos o número. Se retornar 2, tinha lixo.
        if (sscanf(buffer, "%d %c", &value, &extra) == 1 ||
            (sscanf(buffer, "%d", &value) == 1 && buffer[strlen(buffer) - 1] == '\n')) {

            if (value >= 0) { // Garante que a tropa/ID é positiva (ou zero para sair)
                return value;
            }
        }

        printf("Entrada inválida. Digite um número inteiro (>= 0): ");
    }
    return -1; // Falha (EOF)
}

// Escolhe a missão aleatoriamente
void initialize_mission(Mission *mission, const Territory *map, int amount) {
    mission->type = (rand() % MISSION_AMOUNT) + 1;

    if (mission->type == MISSION_DESTROY_ARMY) {
        // Exemplo: sorteia uma cor comum para destruir
        const char *colors[] = {"VERDE", "AZUL", "AMARELO", "ROXO"};
        int random_index = rand() % 4;
        strcpy(mission->target_color, colors[random_index]);
    } else if (mission->type == MISSION_CONQUER_TERRITORIES) {
        mission->territories_needed = 3; // Missão de conquistar 3
    }
}

/********************************************
 * NÍVEL NOVATO: CADASTRO E VISUALIZAÇÃO    *
 *******************************************/

// Opção de preenchimento automático para testar rápido
void auto_fill_territories(Territory *map, int amount) {
    const char *names[] = {"Brasil", "EUA", "China", "India", "Russia"};
    const char *colors[] = {"VERDE", "AZUL", "VERMELHO", "AMARELO", "ROXO"};
    const int troops[] = {5, 8, 4, 6, 7}; // Distribuição básica

    for (int i = 0; i < amount; i++) {
        strcpy(map[i].name, names[i % 5]);
        strcpy(map[i].army_color, colors[i % 5]);
        map[i].troop_count = troops[i % 5] + (rand() % 3); // Pequena variação
    }
    printf("\n--- TERRITÓRIOS PREENCHIDOS AUTOMATICAMENTE ---\n");
}


// Função para preencher todos os territórios
void register_territories(Territory *map, int amount) {
    int choice;
    printf("--- INÍCIO: CADASTRO DOS TERRITÓRIOS ---\n");
    printf("Deseja preencher manualmente (1) ou automaticamente (2)? ");

    // Usando a função segura para ler a escolha (1 ou 2)
    while (1) {
        choice = read_int_safe();
        if (choice == 1 || choice == 2) {
            break;
        }
        printf("Opção inválida. Digite 1 (Manual) ou 2 (Automático): ");
    }

    if (choice == 2) {
        auto_fill_territories(map, amount);
        return;
    }

    // Se for manual, segue o cadastro
    for (int i = 0; i < amount; i++) {
        printf("Território %d/%d:\n", i + 1, amount);

        printf("  Nome: ");
        fgets(map[i].name, sizeof(map[i].name), stdin);
        map[i].name[strcspn(map[i].name, "\n")] = '\0';

        printf("  Cor do exército (Ex: VERDE, AZUL, VERMELHO): ");
        fgets(map[i].army_color, sizeof(map[i].army_color), stdin);
        map[i].army_color[strcspn(map[i].army_color, "\n")] = '\0';

        printf("  Quantidade de tropas: ");
        // Leitura segura de número inteiro, tratando texto
        map[i].troop_count = read_int_safe();

        if (map[i].troop_count == -1) exit(1); // Erro grave
        printf("\n");
    }
    printf("--- FIM: CADASTRO DOS TERRITÓRIOS ---\n");
}

// Exibe o estado atual do mapa de forma legível
void display_map(const Territory *map, int amount) {
    printf("\n=== ESTADO ATUAL DO MAPA ===\n");
    printf("---------------------------------------------\n");
    printf("%-3s | %-20s | %-15s | %-7s\n", "ID", "Nome", "Cor do Exército", "Tropas");
    for (int i = 0; i < amount; i++) {
        // Uso de ponteiro map[i] é a mesma coisa que *(map + i)
        printf("%-3d | %-20s | %-15s | %-7d\n", i + 1, map[i].name, map[i].army_color, map[i].troop_count);
    }
    printf("---------------------------------------------\n");
}

/********************************************
 * NÍVEL AVENTUREIRO: BATALHAS E PONTEIROS  *
 *******************************************/

// Simula o confronto entre o atacante e o defensor
void simulate_battle(Territory *map, int attacker_index, int defender_index) {

    // Regra  : atacante deve ter mais de 1 tropa para atacar
    if (map[attacker_index].troop_count <= 1) {
        printf("ATENÇÃO: Território atacante (%s) precisa de mais de 1 tropa para iniciar o ataque!\n", map[attacker_index].name);
        return;
    }

    printf("\nSimulando a rolagem de dados...\n");
    int attacker_dice = roll_dice();
    int defender_dice = roll_dice();

    printf("Atacante [%s] rolou: %d\n", map[attacker_index].name, attacker_dice);
    printf("Defensor [%s] rolou: %d\n", map[defender_index].name, defender_dice);

    // Empate favorece o atacante (regra do WAR)
    if (attacker_dice >= defender_dice) {
        printf("Vitória do atacante! O defensor (%s) perde 1 tropa.\n", map[defender_index].name);
        map[defender_index].troop_count--;

        if (map[defender_index].troop_count <= 0) {
            // Conquistou! Preciso transferir a cor e 1 tropa.
            printf("Território %s CONQUISTADO! Mudando para a cor %s.\n",
                   map[defender_index].name, map[attacker_index].army_color);
            strcpy(map[defender_index].army_color, map[attacker_index].army_color);
            map[defender_index].troop_count = 1; // Deixa 1 tropa no novo território
        }
    } else {
        printf("Vitória do defensor! O atacante (%s) perde 1 tropa.\n", map[attacker_index].name);
        map[attacker_index].troop_count--; // Atacante perdeu tropa
        if (map[attacker_index].troop_count <= 0) {
            printf("Cuidado! O território atacante ficou sem tropas!\n");
        }
    }
}

/********************************************
 * NÍVEL MESTRE: MISSÕES E MODULARIZAÇÃO    *
 *******************************************/

// Mostra o que o jogador precisa fazer para vencer
void display_mission(const Mission *mission) {
    printf("\n=== MISSÃO MESTRE ATUAL ===\n");
    if (mission->type == MISSION_DESTROY_ARMY) {
        printf("OBJETIVO: Eliminar o exército de cor '%s' do mapa (conquistar todos os territórios deles).\n", mission->target_color);
    } else if (mission->type == MISSION_CONQUER_TERRITORIES) {
        printf("OBJETIVO: Conquistar e possuir %d territórios.\n", mission->territories_needed);
    }
    printf("=============================\n");
}

// Checa se a missão foi cumprida
int check_mission(const Territory *map, int amount, const Mission *mission, const char *player_color) {

    if (mission->type == MISSION_DESTROY_ARMY) {
        // Missão 1: Contar se a cor alvo ainda existe
        int target_army_count = 0;
        for (int i = 0; i < amount; i++) {
            if (strcmp(map[i].army_color, mission->target_color) == 0) {
                target_army_count++;
            }
        }
        // Se a contagem for zero, a missão foi cumprida
        return target_army_count == 0;

    } else if (mission->type == MISSION_CONQUER_TERRITORIES) {
        // Missão 2: Contar quantos territórios pertencem ao jogador
        int player_territory_count = 0;
        for (int i = 0; i < amount; i++) {
            if (strcmp(map[i].army_color, player_color) == 0) {
                player_territory_count++;
            }
        }
        printf("\nVocê possui %d territórios. Precisa de %d.\n", player_territory_count, mission->territories_needed);
        // Se a contagem for maior ou igual ao necessário, missão cumprida
        return player_territory_count >= mission->territories_needed;
    }

    return 0; // Se não for nenhuma, não foi cumprida
}


// Função principal (main)
int main() {
    // Configurações iniciais
    setlocale(LC_ALL, "pt_BR.UTF-8");
    srand(time(NULL));

    // Alocação dinâmica com calloc (Nível Aventureiro)
    Territory *map = (Territory *)calloc(TERRITORY_AMOUNT, sizeof(Territory));

    if (map == NULL) {
        perror("Ops! Erro ao alocar memória. Falha no calloc.");
        return 1;
    }

    Mission current_mission;
    char player_initial_color[20] = ""; // Cor que vamos usar para a missão de conquista
    int running = 1;

    // --- CADASTRO INICIAL (NÍVEL NOVATO) ---
    register_territories(map, TERRITORY_AMOUNT);
    display_map(map, TERRITORY_AMOUNT);

    // Define a cor do primeiro território como a cor do "jogador" para o sistema de missão
    if (TERRITORY_AMOUNT > 0) {
         strcpy(player_initial_color, map[0].army_color);
    }
    initialize_mission(&current_mission, map, TERRITORY_AMOUNT);

    // --- LOOP PRINCIPAL DO JOGO (NÍVEL MESTRE) ---
    int choice;

    while (running) {
        printf("\n=== MENU PRINCIPAL ===\n");
        printf("1. Atacar\n");
        printf("2. Verificar Missão\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");

        // Usando a função segura de leitura
        choice = read_int_safe();

        if (choice == -1) { // Erro fatal
            running = 0;
            break;
        }

        switch (choice) {
            case 1: { // Opção ATACAR
                display_map(map, TERRITORY_AMOUNT);
                printf("\n--- FASE DE ATAQUE ---\n");
                int attacker_id, defender_id;

                printf("ID do Atacante (1-%d): ", TERRITORY_AMOUNT);
                attacker_id = read_int_safe();
                if (attacker_id == 0) break; // Volta ao menu

                printf("ID do Defensor (1-%d): ", TERRITORY_AMOUNT);
                defender_id = read_int_safe();
                if (defender_id == 0) break; // Volta ao menu

                // Validação de IDs e limites
                if (attacker_id == defender_id ||
                    attacker_id < 1 || defender_id < 1 ||
                    attacker_id > TERRITORY_AMOUNT || defender_id > TERRITORY_AMOUNT) {
                    printf("Escolha de IDs inválida! Tente novamente.\n");
                    continue;
                }

                // Não pode atacar o próprio território
                 if (strcmp(map[attacker_id - 1].army_color, map[defender_id - 1].army_color) == 0) {
                    printf("Não pode atacar seu próprio território. Escolha um inimigo!\n");
                    continue;
                }

                // Simulação da batalha (NÍVEL AVENTUREIRO)
                simulate_battle(map, attacker_id - 1, defender_id - 1);
                display_map(map, TERRITORY_AMOUNT);
                break;
            }
            case 2: { // Opção VERIFICAR MISSÃO (NÍVEL MESTRE)
                display_mission(&current_mission);
                if (check_mission(map, TERRITORY_AMOUNT, &current_mission, player_initial_color)) {
                    printf("🎉 Uau! Missão MESTRE cumprida! Você VENCEU o jogo!\n");
                    running = 0;
                } else {
                    printf("Ainda não foi dessa vez. Continue a conquista!\n");
                }
                break;
            }
            case 0: { // SAIR
                running = 0;
                printf("Encerrando o jogo. Até a próxima, comandante!\n");
                break;
            }
            default: {
                printf("Opção do menu inválida. Tente de novo.\n");
                break;
            }
        }
    }

    // Liberação de memória
    if (map != NULL) {
        free(map);
        map = NULL;
    }

    return 0;
}
