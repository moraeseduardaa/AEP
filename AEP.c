#include <stdio.h>
#include <stdlib.h>                                       
#include <string.h>
#include <locale.h>

#define CHAVE 3
#define NOME 40
#define SENHA 40                                             
#define ARQUIVO_USUARIOS "usuarios.txt"

typedef struct {
    int id;
    char nome[NOME];
    char senha[SENHA];                                       
} Usuario;

Usuario usuarios[40];
int cont = 0;

void criptografar(char *texto, int chave) {
	int i;
    for (i = 0; texto[i] != '\0'; i++) {                 
        texto[i] += chave;
    }
}

void descriptografar(char *texto, int chave) {
	int i;
    for (i = 0; texto[i] != '\0'; i++) {                  
        texto[i] -= chave;
    }
}

void salvarUsuariosEmArquivo() {
    FILE *file = fopen(ARQUIVO_USUARIOS, "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo para salvar!\n");
        return;
    }
    int i;
    for (i = 0; i < cont; i++) {                          
        char senhaCriptografada[SENHA];
        
        strcpy(senhaCriptografada, usuarios[i].senha);
        criptografar(senhaCriptografada, CHAVE);

        fprintf(file, "%d,%s,%s\n", usuarios[i].id, usuarios[i].nome, senhaCriptografada);
    }
    fclose(file);
}

void loadUsuariosDoArquivo() {
    FILE *file = fopen(ARQUIVO_USUARIOS, "r");
    if (file == NULL) {
        return;
    }                                                     

    while (fscanf(file, "%d,%39[^,],%39s", &usuarios[cont].id, usuarios[cont].nome, usuarios[cont].senha) == 3) {
        descriptografar(usuarios[cont].senha, CHAVE);
        cont++;
    }
    fclose(file);
}

int verificarNomeDuplicado(const char *nome) {
	int i;
    for (i = 0; i < cont; i++) {
        if (strcmp(usuarios[i].nome, nome) == 0) {   
            return 1;
        }
    }
    return 0;
}

int main() {
    setlocale(LC_ALL, "Portuguese");
    int opcao, id, encontrado, i, j;                
    char senha[SENHA], confirmacao;

    loadUsuariosDoArquivo();

    do {
        system("cls");

        printf("Menu:\n");
        printf("1. Inclusão de novos usuários\n");
        printf("2. Alteração de usuários\n");
        printf("3. Exclusão de usuários\n");          
        printf("4. Listagem de usuários\n");
        printf("5. Sair do programa\n");
        printf("\nEscolha uma opção: ");
        scanf("%d", &opcao);
// kevin
        switch (opcao) {
            case 1:
                if (cont < 40) {
                    usuarios[cont].id = cont + 1;
                    printf("\nId do Usuário: %d \n", usuarios[cont].id);
                    printf("\nInforme o nome do usuário: ");
                    scanf(" %39s", usuarios[cont].nome);

                    if (verificarNomeDuplicado(usuarios[cont].nome)) {
                        printf("Nome já existe! Escolha outro nome.\n");
                    } else {
                        printf("Informe a senha do usuário: ");
                        scanf(" %39s", usuarios[cont].senha);
                        cont++;
                        salvarUsuariosEmArquivo();
                        printf("\nUsuário adicionado com sucesso!\n");
                    }
                } else {
                    printf("\nLimite máximo de usuários atingido!\n");
                }
                break;

            case 2:
                printf("\nInforme o ID do usuário que você deseja alterar: ");
                scanf("%d", &id);
                encontrado = 0;

                for (i = 0; i < cont; i++) {
                    if (usuarios[i].id == id) {
                        encontrado = 1;
                        printf("\nUsuário encontrado:\n");
                        printf("Nome: %s\n", usuarios[i].nome);

                        printf("Informe a senha do usuário: ");
                        scanf(" %39s", senha);

                        if (strcmp(senha, usuarios[i].senha) == 0) {
                            printf("\nNovo nome do usuário: ");
                            scanf(" %39s", usuarios[i].nome);
                            printf("Nova senha do usuário: ");
                            scanf(" %39s", usuarios[i].senha);
                            salvarUsuariosEmArquivo();
                            printf("\nUsuário alterado com sucesso!\n");
                        } else {
                            printf("\nSenha incorreta!\n");
                        }
                        break;
                    }
                }

                if (!encontrado) {
                    printf("\nUsuário não encontrado!\n");
                }
                break;

            case 3:
                printf("\nInforme o ID do usuário que deseja excluir: ");
                scanf("%d", &id);
                encontrado = 0;

                for (i = 0; i < cont; i++) {
                    if (usuarios[i].id == id) {
                        encontrado = 1;
                        printf("\nUsuário encontrado:\n");
                        printf("Nome: %s\n", usuarios[i].nome);

                        printf("Informe a senha do usuário para confirmação: ");
                        scanf(" %39s", senha);

                        if (strcmp(senha, usuarios[i].senha) == 0) {
                            printf("\nTem certeza que deseja excluir este usuário? (S/N): ");
                            scanf(" %c", &confirmacao);

                            if (confirmacao == 's' || confirmacao == 'S') {
                                for (j = i; j < cont - 1; j++) {
                                    usuarios[j] = usuarios[j + 1];
                                }
                                cont--;
                                salvarUsuariosEmArquivo();
                                printf("\nUsuário excluído com sucesso!\n");
                            } else {
                                printf("\nExclusão cancelada.\n");
                            }
                        } else {
                            printf("\nSenha incorreta! Exclusão não permitida.\n");
                        }
                        break;
                    }
                }

                if (!encontrado) {
                    printf("\nUsuário não encontrado!\n");
                }
                break;

            case 4:
                if (cont == 0) {
                    printf("Nenhum usuário cadastrado!\n");
                } else {
                    printf("\nLista de Usuários:\n");
                    for (i = 0; i < cont; i++) {
                        printf("ID: %d, Nome: %s\n", usuarios[i].id, usuarios[i].nome);
                    }
                }
                break;

            case 5:
                printf("\nTem certeza que deseja sair? (S/N): ");
                scanf(" %c", &confirmacao);
                if (confirmacao == 's' || confirmacao == 'S') {
                    printf("\nSaindo do programa...\n");
                } else {
                    opcao = 0;
                }
                break;

            default:
                printf("Opção inválida! Por favor, escolha uma opção válida.\n");
        }

        if (opcao != 5) {
            printf("\nPressione qualquer tecla para continuar\n");
            getchar();
            getchar();
        }

    } while (opcao != 5);

    return 0;
}
