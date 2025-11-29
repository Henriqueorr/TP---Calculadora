#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "expressao.h"

int main() {
    // Testes conforme especificação
    char *testes[][3] = {
        {"3 4 + 5 *", "(3+4)*5", "35"},
        {"7 2 * 4 +", "7*2+4", "18"},
        {"8 5 2 4 + * +", "8+(5*(2+4))", "38"},
        {"6 2 / 3 + 4 *", "(6/2+3)*4", "24"},
        {"9 5 2 8 * 4 + * +", "9+(5*(4+8*2))", "109"},
        {"2 3 + log 5 /", "log(2+3)/5", "0.14"},
        {"10 log 3 ^ 2 +", "log(10)^3+2", "3"},
        {"45 60 + 30 cos *", "(45+60)*cos(30)", "90.93"},
        {"0.5 45 sen 2 ^ +", "sen(45)^2+0.5", "1"}
    };
    
    int numTestes = sizeof(testes) / sizeof(testes[0]);
    
    printf("=== AVALIADOR DE EXPRESSÕES ===\n\n");
    
    for (int i = 0; i < numTestes; i++) {
        printf("Teste %d:\n", i + 1);
        printf("Pós-fixa: %s\n", testes[i][0]);
        
        // Testa conversão para infixa
        char *infixa = getFormaInFixa(testes[i][0]);
        if (infixa != NULL) {
            printf("Infixa obtida: %s\n", infixa);
            printf("Infixa esperada: %s\n", testes[i][1]);
            free(infixa);
        } else {
            printf("ERRO: Não foi possível converter para infixa\n");
        }
        
        // Testa cálculo do valor
        float valor = getValorPosFixa(testes[i][0]);
        printf("Valor calculado: %.2f\n", valor);
        printf("Valor esperado: %s\n", testes[i][2]);
        printf("\n");
    }
    
    // Teste adicional com entrada do usuário
    printf("=== TESTE INTERATIVO ===\n");
    printf("Digite uma expressão pós-fixa (ou 'sair' para encerrar):\n");
    
    char entrada[512];
    while (1) {
        printf("\n> ");
        if (fgets(entrada, sizeof(entrada), stdin) == NULL) break;
        
        // Remove newline
        entrada[strcspn(entrada, "\n")] = 0;
        
        if (strcmp(entrada, "sair") == 0) break;
        if (strlen(entrada) == 0) continue;
        
        char *infixa = getFormaInFixa(entrada);
        if (infixa != NULL) {
            printf("Forma infixa: %s\n", infixa);
            free(infixa);
        } else {
            printf("ERRO: Expressão inválida\n");
        }
        
        float valor = getValorPosFixa(entrada);
        printf("Valor: %.4f\n", valor);
    }
    
    printf("\nPrograma encerrado.\n");
    return 0;
}