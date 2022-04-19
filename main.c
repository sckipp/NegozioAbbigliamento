/*
 * TO - DO SE FACCIO LOGIN ED ESCO DALL'ACCOUNT, SE RIOLGGO NON MI FA SCEGLIERE L'ACCOUNT (NECESSARIA FREE DI UTENTELOGGATO)
 *
 */
#include <stdio.h>
#include <stdlib.h>

//FILE progetto.h
#define MAX 50
#define TAGLIE 4

typedef struct accountUtente_Lista{
    char *nomeUtente;
    char *password;
    float bilancio_conto;
    struct accountUtente_Lista *next;
}accountUtenteLista;

typedef struct abbigliamentoLista{
    int id;
    char *nomeAbbigliamento;
    int taglieDisp[TAGLIE];
    float prezzo;
    struct abbigliamentoLista *next;
}abbigliamentoLista;

typedef struct scarpelista{
    int id;
    char *nomeScarpe;
    int numeroDisp[10];
    float prezzo;
    struct scarpelista *next;
}scarpeLista;

accountUtenteLista *creaNodoAccount (char *nomeUtente, char *password);
accountUtenteLista *inserimentoTestaAccount (accountUtenteLista *accountLista, char *nomeUtente, char *password);
void allocazioneAccountUtenteLista (char *nome, char *password, accountUtenteLista *tmp);
void popolamentoFileAccount(char *nome, char *password);
void registrazioneUtente();
accountUtenteLista *loginUtente (accountUtenteLista  *utenteLoggato, char *nomeUtente, char *password, accountUtenteLista *accountLista);
accountUtenteLista *inserimentoAccountLista (accountUtenteLista *accountLista);
accountUtenteLista *freeAccount (accountUtenteLista *testa);
abbigliamentoLista *freeAbbigl (abbigliamentoLista *testa);
scarpeLista *freeScarpe (scarpeLista *testa);
accountUtenteLista *confrontoCredenzialiConDB (accountUtenteLista *accountLista, char *nomeUtente, char *password);
void *operazioni (accountUtenteLista *utenteLoggato);
void allocazioneAbbigliamentoLista (char *nome,  abbigliamentoLista *tmp);
void allocazioneScarpeLista (char *nome, scarpeLista *tmp);
void stampaScarpeLista (scarpeLista *scarpe);
void stampaAbbigliamentoLista (abbigliamentoLista *abbigliamento);
//FILE funzioni.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//GESTIONE DELLE LISTE
//lista degli account
accountUtenteLista *creaNodoAccount (char *nomeUtente, char *password){
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
accountUtenteLista *inserimentoTestaAccount (accountUtenteLista *accountLista, char *nomeUtente, char *password){
    accountUtenteLista *tmp = NULL;

    if(accountLista == NULL)
        return creaNodoAccount(nomeUtente, password);
    tmp = creaNodoAccount(nomeUtente, password);
    tmp ->next = accountLista;
    return tmp;
}
accountUtenteLista *inserimentoAccountLista (accountUtenteLista *accountLista){

    FILE *fp = NULL;
    char stringa[MAX];
    char nome[MAX], password[MAX];

    fp = fopen("account.txt", "r");
    accountLista = freeAccount(accountLista); //Causa crash
    while(fgets(stringa, MAX, fp) != NULL){
        sscanf(stringa, "%s %s", nome, password);
        accountLista = inserimentoTestaAccount(accountLista, nome, password);
    }
    fclose(fp);
    return accountLista;
}
void allocazioneAccountUtenteLista (char *nome, char *password, accountUtenteLista *tmp){
    int dimN = 0, dimP = 0;
    dimN = strlen(nome);
    dimP = strlen(password);
    tmp ->nomeUtente = malloc(dimN * sizeof (char));
    tmp ->password = malloc(dimP * sizeof (char));

}
accountUtenteLista *freeAccount (accountUtenteLista *testa){
    accountUtenteLista *tmp = NULL;

    while (testa != NULL) {
        tmp = testa;
        testa = testa->next;
        free(tmp);
    }
    return NULL;
}

//lista dell'abbigliamento
abbigliamentoLista *creaNodoAbbigliamento (char *nomeAbbigliamento, float prezzo, int id){
    /*srand(time(NULL));
    rand();*/
    abbigliamentoLista *tmp = NULL;
    tmp = malloc (sizeof (abbigliamentoLista));
    if(tmp == NULL)
        return NULL;
    else{
        tmp ->next = NULL;
        allocazioneAbbigliamentoLista(nomeAbbigliamento, tmp);
        strcpy(tmp->nomeAbbigliamento,nomeAbbigliamento);
        tmp ->id = id;
        tmp ->prezzo = prezzo;
        /*for(int i = 0; i < TAGLIE; i++)
            tmp ->taglieDisp[i] = rand()%5;*/
    }
    return tmp;
}
abbigliamentoLista *inserimentoCodaAbbigliamento (abbigliamentoLista *abbigliamento, char *nomeAbbigliamento, float prezzo, int id){
    abbigliamentoLista *tmp = abbigliamento;

    if(tmp == NULL)
        return creaNodoAbbigliamento(nomeAbbigliamento, prezzo, id);
    while(tmp ->next != NULL)
        tmp = tmp ->next;
    tmp -> next = creaNodoAbbigliamento(nomeAbbigliamento, prezzo, id);
    return abbigliamento;
}
void allocazioneAbbigliamentoLista (char *nome,  abbigliamentoLista *tmp){
    int dimN = 0;
    dimN = strlen(nome);
    tmp ->nomeAbbigliamento = malloc(dimN * sizeof (char));

}
abbigliamentoLista *popolamentoAbbigliamentoLista(abbigliamentoLista *abbigliamento, int *contatore){
    FILE *fp = NULL;
    char stringa[100];
    char nome[100];
    float prezzo;
    int id =0;
    fp = fopen("abiti.txt", "r");
    abbigliamento = freeAbbigl(abbigliamento);
    while(fgets(stringa, 100, fp) != NULL){
        sscanf(stringa, "%f %[^\n]s", &prezzo, nome);
        id++;
        abbigliamento = inserimentoCodaAbbigliamento(abbigliamento, nome, prezzo, id);
    }
    *contatore = id;
    return abbigliamento;
}
void stampaAbbigliamentoLista (abbigliamentoLista *abbigliamento){
    char tagl[]={'S', 'M', 'L', 'L'};
    printf("\nStampa della lista abbigliamento\n"); //Debugging purpose
    while(abbigliamento != NULL){
        printf("%d - %s %.2f\n",abbigliamento ->id, abbigliamento ->nomeAbbigliamento, abbigliamento ->prezzo);
        /*for(int i  = 0; i < TAGLIE; i++)
            printf("%c: %d", tagl[i], abbigliamento ->taglieDisp[i]);
        printf("\n");*/
        abbigliamento = abbigliamento ->next;
    }
    printf("\n");
}
abbigliamentoLista *freeAbbigl (abbigliamentoLista *testa){
    abbigliamentoLista *tmp = NULL;

    while (testa != NULL) {
        tmp = testa;
        testa = testa->next;
        free(tmp);
    }
    return NULL;
}

//lista delle scarpe
scarpeLista *creaNodoScarpe (char *nomeScarpe, float prezzo, int id){
    scarpeLista *tmp = NULL;
    tmp = malloc (sizeof (scarpeLista));
    if(tmp == NULL)
        return NULL;
    else{
        tmp ->next = NULL;
        allocazioneScarpeLista(nomeScarpe, tmp);
        strcpy(tmp->nomeScarpe,nomeScarpe);
        tmp ->id = id;
        tmp ->prezzo = prezzo;
    }
    return tmp;
}
scarpeLista *inserimentoCodaScarpe (scarpeLista *scarpe, char *nomeScarpe, float prezzo, int id){
    scarpeLista *tmp = scarpe;

    if(scarpe == NULL)
        return creaNodoScarpe(nomeScarpe, prezzo, id);
    while(tmp ->next != NULL)
        tmp = tmp ->next;
    tmp -> next = creaNodoScarpe(nomeScarpe, prezzo, id);
    return scarpe;
}
scarpeLista *popolamentoScarpeLista(scarpeLista *scarpe, int contatore){
    FILE *fp = NULL;
    char stringa[100];
    char nome[100];
    float prezzo;
    int id = contatore;
    printf("stampa contatore %d", contatore);
    fp = fopen("scarpe.txt", "r");
    scarpe = freeScarpe(scarpe);
    while(fgets(stringa, 100, fp) != NULL){
        sscanf(stringa, "%f %[^\n]s", &prezzo, nome);
        contatore++;
        scarpe = inserimentoCodaScarpe(scarpe, nome, prezzo, contatore);
    }
    return scarpe;
}
void stampaScarpeLista (scarpeLista *scarpe){
    printf("\nStampa della lista scarpe\n"); //Debugging purpose
    while(scarpe != NULL){
        printf("%d - %s %.2f\n",scarpe ->id, scarpe ->nomeScarpe, scarpe ->prezzo);
        scarpe = scarpe ->next;
    }
    printf("\n");
}
void allocazioneScarpeLista (char *nome, scarpeLista *tmp){
    int dimN = 0;
    dimN = strlen(nome);
    tmp ->nomeScarpe = malloc(dimN * sizeof (char));

}
scarpeLista *freeScarpe (scarpeLista *testa){
    scarpeLista *tmp = NULL;

    while (testa != NULL) {
        tmp = testa;
        testa = testa->next;
        free(tmp);
    }
    return NULL;
}

//gestione account utente
accountUtenteLista *confrontoCredenzialiConDB (accountUtenteLista *accountLista, char *nomeUtente, char *password){
    accountUtenteLista *tmp = accountLista;

    while(tmp != NULL){
        if((strcmp(nomeUtente, tmp->nomeUtente) == 0) && (strcmp(password, tmp ->password) == 0)) {
            return tmp;
        }
        tmp = tmp->next;
    }
        return NULL;
}

//GESTIONE DEI FILE
void popolamentoFileAccount(char *nome, char *password){
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
    popolamentoFileAccount(nome, password);
}
void gestioneMagazzino(){
    char nomeCapo[MAX];
    float prezzo;
    char nomeScarpe[MAX];
    FILE *abiti = NULL;
    FILE *scarpe = NULL;

    int scelta = 1;

    printf("Seleziona un operazione (0 per annullare l'operazione)\n1 - Per inserire capi d'abbgliamento\n2 - Per inserire scarpe\nScelta : ");
    scanf("%d", &scelta);
    fflush(stdin);
    while(scelta != 0) {
        if(scelta == 1) {
            abiti = fopen("abiti.txt", "a");
            printf("Inserisci il nome dell'abbigliamento\n");
            fgets(nomeCapo, MAX, stdin);
            printf("Inserisci il prezzo\n");
            scanf("%f", &prezzo);
            fprintf(abiti, "%f %s", prezzo, nomeCapo);
            fclose(abiti);
        }
        if(scelta == 2) {
            scarpe = fopen("scarpe.txt", "a");
            printf("Inserisci il nome delle scarpe\n");
            fgets(nomeScarpe, MAX, stdin);
            printf("Inserisci il prezzo\n");
            scanf("%f", &prezzo);
            fprintf(scarpe, "%f %s", prezzo, nomeScarpe);
            fclose(scarpe);
        }
        printf("Seleziona un operazione (0 per annullare l'operazione)\n1 Per inserire capi d'abbgliamento\n2 Per inserire scarpe\nScelta : ");
        scanf("%d", &scelta);
        fflush(stdin);
        if(scelta == 0) {
            printf("Uscita dal menu in corso...\n");
            system("pause");
        }
    }

}

//GESTIONE INTERFACCIA
accountUtenteLista *loginUtente (accountUtenteLista  *utenteLoggato, char *nomeUtente, char *password, accountUtenteLista *accountLista){

    printf("Inserisci lo username: ");
    fgets(nomeUtente, MAX, stdin);
    nomeUtente[strcspn(nomeUtente, "\n")] = 0;
    printf("\nInserisci la password: ");
    fgets(password, MAX, stdin);
    password[strcspn(password, "\n")] = 0;

    utenteLoggato = confrontoCredenzialiConDB(accountLista, nomeUtente, password);

    if(utenteLoggato != NULL) {
        printf("\nUtente %s %s riconosciuto con successo\n", utenteLoggato->nomeUtente, utenteLoggato->password);
        return utenteLoggato;
    }
    else {
        printf("Hai inserito le credenziali sbagliate\nReinseriscile\n");
        return NULL;
    }
}
void acquistoProdotti(abbigliamentoLista *abbigliamento, scarpeLista *scarpe, int contatore){
    int scelta;
    abbigliamentoLista *tmpAbbigliamento = abbigliamento;
    scarpeLista *tmpScarpe = scarpe;

    printf("Scelta:");
    scanf("%d", &scelta);

    if(scelta <= contatore) {
        while (tmpAbbigliamento != NULL) {
            if (scelta == tmpAbbigliamento->id)
                printf("Hai acquistato %s", tmpAbbigliamento->nomeAbbigliamento);
            tmpAbbigliamento = tmpAbbigliamento->next;
        }
    }else{
        while(tmpScarpe != NULL){
            if(scelta == tmpScarpe ->id)
                printf("Hai acquistato %s", tmpScarpe ->nomeScarpe);
            tmpScarpe = tmpScarpe ->next;
        }
    }
}
void *operazioni (accountUtenteLista *utenteLoggato){

    scarpeLista *scarpe = NULL;
    abbigliamentoLista  *abbigliamento = NULL;
    int contatore;
    int scelta;
    float importo;
    do {
        printf("1 - Mostra capi d'abbigliamento\n2 - Ricarica il tuo conto virtuale\n3 - Preleva soldi dal tuo conto virtuale\n"
               "4 - Carrello\n0 - Logout\nInserisci operazione : ");
        scanf("%d", &scelta);
        fflush(stdin);
        switch (scelta) {
            case 1:
                printf("DEBUGGING PURPOSE\n\n\n");
                abbigliamento = popolamentoAbbigliamentoLista(abbigliamento, &contatore);
                scarpe = popolamentoScarpeLista(scarpe, contatore);
                stampaAbbigliamentoLista(abbigliamento);
                stampaScarpeLista(scarpe);
                acquistoProdotti(abbigliamento, scarpe, contatore); //INSERIRE FUNZIONE DI ACQUISTO ARTICOLO
                break;
            case 2:
                printf("Inserisci l'importo");
                scanf("%f", &importo);
                utenteLoggato ->bilancio_conto += importo;
                break;
            case 3: {
                printf("Preleva i soldi dal tuo conto\n");
                scanf("%f", &importo);
                if (utenteLoggato ->bilancio_conto >= importo)
                    utenteLoggato ->bilancio_conto -= importo;
                else
                    printf("Impossibile completare l'operazione...\nBilancio non sufficiente\n");
                break;
            }
            case 0:
                printf("Logout dall'account in corso...\n");
                break;
            default:
                printf("Hai inserito un operazione sconosciuta...\nReinserire l'operazione\n");
        }
        system("pause");
    }while(scelta != 0);

     //scarpe = freeScarpe(scarpe);
    // abbigliamento = freeAbbigl(abbigliamento);
}

//FILE main.c
int main() {

    accountUtenteLista *accountLista = NULL;  //lista che tiene salvati i dati di tutti gli account
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
                while(utenteloggato == NULL)//Inserisci le credenziali ogni volta che fallisce il login.
                    utenteloggato = loginUtente(utenteloggato, nome, pass, accountLista);
                operazioni(utenteloggato);
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
    }while(scelta != -1);

}
