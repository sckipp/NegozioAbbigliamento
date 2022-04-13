/*
 * TO - DO C'E' UN COMMENTO AL LOGIN UTENTE DA CONTROLLARE.
 * IL FREELISTA CONTINUO MI ROMPE IL PROGRAMMA, CERCARE DI RISOLVERE ASAP
 *
 */
#include <stdio.h>
#include <stdlib.h>

//FILE progetto.h
#define MAX 30
typedef struct accountUtente_Lista{
    char *nomeUtente;
    char *password;
    float bilancio_conto;
    struct accountUtente_Lista *next;
}accountUtenteLista;

typedef struct abbigliamento{
    char *nomeAbbigliamento;
    int *taglieDisp;
}abbigliamentoLista;

typedef struct scarpe{
    char *nomeScarpe;
    int *numeroDisp;
}scarpeLista;

accountUtenteLista *creaNodo (char *nomeUtente, char *password);
accountUtenteLista *inserimentoTesta (accountUtenteLista *accountLista, char *nomeUtente, char *password);
void allocazioneAccountUtenteLista (char *nome, char *password, accountUtenteLista *tmp);
void popolamentoFile(char *nome, char *password);
void registrazioneUtente();
accountUtenteLista *loginUtente (accountUtenteLista  *utenteLoggato, char *nomeUtente, char *password, accountUtenteLista *accountLista);
accountUtenteLista *inserimentoAccountLista (accountUtenteLista *accountLista);
void freeLista (accountUtenteLista *testa);
accountUtenteLista *confrontoCredenzialiConDB (accountUtenteLista *accountLista, char *nomeUtente, char *password);

//FIlE funzioni.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//gestione delle liste
accountUtenteLista *creaNodo (char *nomeUtente, char *password){
    accountUtenteLista *tmp = NULL;
    tmp = malloc (sizeof (accountUtenteLista));
    if(tmp == NULL)
        return NULL;
    else{
        tmp ->next = NULL;
        allocazioneAccountUtenteLista(nomeUtente,password, tmp);
        strcpy(tmp->nomeUtente,nomeUtente);
        strcpy(tmp ->password,password);
    }
    return tmp;
}
accountUtenteLista *inserimentoTesta (accountUtenteLista *accountLista, char *nomeUtente, char *password){
    accountUtenteLista *tmp = NULL;

    if(accountLista == NULL)
        return creaNodo(nomeUtente, password);
    tmp = creaNodo(nomeUtente, password);
    tmp ->next = accountLista;
    return tmp;
}
void allocazioneAccountUtenteLista (char *nome, char *password, accountUtenteLista *tmp){
    int dimN = 0, dimP = 0;
    dimN = strlen(nome);
    dimP = strlen(password);
    tmp ->nomeUtente = malloc(dimN * sizeof (char));
    tmp ->password = malloc(dimP * sizeof (char));

}//Dovrebbe andare
void stampaNodi (accountUtenteLista *accountLista){
    printf("Stampa della lista\n"); //Bugging purpose
    while(accountLista != NULL){
        printf("Nome Utente %s Password %s\n", accountLista ->nomeUtente, accountLista ->password);
        accountLista = accountLista ->next;
    }
}//Debugging purpose
accountUtenteLista *inserimentoAccountLista (accountUtenteLista *accountLista){
    accountUtenteLista *tmp = NULL;

    FILE *fp = NULL;
    char stringa[MAX];
    char nome[MAX], password[MAX];

    fp = fopen("account.txt", "r");
    while(fgets(stringa, MAX, fp) != NULL){
        sscanf(stringa, "%s %s", nome, password);
        tmp = inserimentoTesta(tmp, nome, password);
    }
    accountLista = tmp;
    freeLista(tmp);
    if(tmp == NULL)
        printf("tmp e' null, problema risolto\n");
    else
        printf("%s", tmp->nomeUtente);
    fclose(fp);
    return accountLista;
}
void freeLista (accountUtenteLista *testa){
    accountUtenteLista *tmp = NULL;

        while (testa != NULL) {
            tmp = testa;
            testa = testa->next;
            free(tmp);
        }

}

accountUtenteLista *confrontoCredenzialiConDB (accountUtenteLista *accountLista, char *nomeUtente, char *password){
    accountUtenteLista *tmp = accountLista;
    while(tmp != NULL){
        if((strcmp(nomeUtente, tmp->nomeUtente) == 0) && (strcmp(password, tmp ->password) == 0)) {
            return tmp;
        }
        else
        tmp = tmp ->next;
    }
        return NULL;
}

//gestione dei file
void popolamentoFile(char *nome, char *password){
    FILE *fp = NULL;

    fp = fopen("account.txt", "a");
    fprintf(fp, "%s %s\n", nome, password);
    fclose(fp);
}
void registrazioneUtente(){

    char nome[MAX], password[MAX];

    printf("Inserisci il tuo nome\n");
    fgets(nome, MAX, stdin);
    printf("Inserisci la tua password\n");
    fgets(password, MAX, stdin);

    nome[strcspn(nome, "\n")] = 0; //Si mangia lo \n della fgets
    password[strcspn(password, "\n")] = 0;
    popolamentoFile(nome, password);
}
void gestioneMagazzino(){
    FILE *abiti = NULL;
    FILE *scarpe = NULL;

    abiti = fopen("abiti.txt", "a");
    fclose(abiti);

    scarpe = fopen("scarpe.txt", "a");
    fclose(scarpe);
}

//Gestione interfaccia
accountUtenteLista *loginUtente (accountUtenteLista  *utenteLoggato, char *nomeUtente, char *password, accountUtenteLista *accountLista){

    printf("Inserisci lo username: ");
    fgets(nomeUtente, MAX, stdin);
    nomeUtente[strcspn(nomeUtente, "\n")] = 0;
    printf("\nInserisci la password: ");
    fgets(password, MAX, stdin);
    password[strcspn(password, "\n")] = 0;

    utenteLoggato = confrontoCredenzialiConDB(accountLista, nomeUtente, password); //funziona anche senza l'assegnazione - - - Domandare ad Andrea

    if(utenteLoggato != NULL)
        printf("\nUtente %s %s riconosciuto con successo\n", utenteLoggato->nomeUtente, utenteLoggato ->password);
    else
        printf("Hai inserito le credenziali sbagliate\n\n");
    return utenteLoggato;
}
//FILE main.c
int main() {

    accountUtenteLista *accountLista = NULL; //lista che tiene salvati i dati di tutti gli account
    accountUtenteLista *utenteloggato = NULL;//lista che tiene salvati i dati dell'account che ha effettuato l'accesso
    char nome[MAX];
    char pass[MAX];
    int scelta;
    do {
    printf("Benvenuto nel negozio virtuale!!\n"
                  "Scegli quale operazione effettuare (-1 per uscire dal negozio)\n"
                  "1 - Per l'accesso utente\n"
                  "2 - Per la registrazione utente\n"
                  "3 - Per l'accesso Admin\n");
    printf("Dunque, inserisci l'operazione da effettuare: ");
    scanf("%d", &scelta);
    fflush(stdin);

        switch (scelta) {
            case 1:
                printf("Inserisci lo username e la password per accedere al tuo profilo\n");
                accountLista = inserimentoAccountLista(accountLista);
                utenteloggato = loginUtente(utenteloggato, nome, pass, accountLista);
                break;
            case 2:
                printf("Hai selezionato la registrazione utente\n");
                registrazioneUtente();
                break;
            case 3:
                printf("Hai selezionato l'accesso amministratore\n");
                gestioneMagazzino();
                break;
            case -1:
                printf("Chiusura negozio in corso...\nTorna a trovarci presto!!!\n");
                break;
            default:
                printf("\nHai inserito un operazione sconosciuta...\nReinserisci l'operazione da eseguire.\n");
                break;
        }
        system("pause");
    }while(scelta != -1);

}
