/*
 * TO - DO
 * SE FACCIO LOGIN ED ESCO DALL'ACCOUNT, SE RIOLGGO NON MI FA SCEGLIERE L'ACCOUNT (NECESSARIA FREE DI UTENTELOGGATO)
 * IMPLEMENTARE COSA FARE IN CASO DI SALDO NON DISPONIBILE PER L'ACQUISTO
 * IMPLEMENTARE CARRELLO
 */
#include <stdio.h>
#include <stdlib.h>

//FILE progetto.h
#define MAX 50
#define TAGLIE 3

typedef struct accountUtente_Lista{
    char *nomeUtente;
    char *password;
    float bilancio_conto;
    struct accountUtente_Lista *next;
}accountUtenteLista;

typedef struct abbigliamentoLista{
    int id;
    char *nomeAbbigliamento;
    int disponibilita;
    float prezzo;
    struct abbigliamentoLista *next;
}abbigliamentoLista;

typedef struct scarpelista{
    int id;
    char *nomeScarpe;
    int disponibilita;
    float prezzo;
    struct scarpelista *next;
}scarpeLista;

accountUtenteLista *creaNodoAccount (char *nomeUtente, char *password, float saldo);
accountUtenteLista *inserimentoTestaAccount (accountUtenteLista *accountLista, char *nomeUtente, char *password, float saldo);
void allocazioneAccountUtenteLista (char *nome, char *password, accountUtenteLista *tmp);
void popolamentoFileAccount(char *nome, char *password, float saldo);
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
accountUtenteLista *creaNodoAccount (char *nomeUtente, char *password, float saldo){
    accountUtenteLista *tmp = NULL;
    tmp = malloc (sizeof (accountUtenteLista));
    if(tmp == NULL)
        return NULL;
    else{
        tmp ->next = NULL;
        allocazioneAccountUtenteLista(nomeUtente,password, tmp);
        strcpy(tmp->nomeUtente,nomeUtente);
        strcpy(tmp ->password,password);
        tmp ->bilancio_conto = saldo;
    }
    return tmp;
}
accountUtenteLista *inserimentoTestaAccount (accountUtenteLista *accountLista, char *nomeUtente, char *password, float saldo){
    accountUtenteLista *tmp = NULL;

    if(accountLista == NULL)
        return creaNodoAccount(nomeUtente, password, saldo);
    tmp = creaNodoAccount(nomeUtente, password, saldo);
    tmp ->next = accountLista;
    return tmp;
}
accountUtenteLista *inserimentoAccountLista (accountUtenteLista *accountLista){

    FILE *fp = NULL;
    char stringa[MAX];
    char nome[MAX], password[MAX];
    float saldo;
    fp = fopen("account.txt", "r");
    accountLista = freeAccount(accountLista); //Causa crash
    while(fgets(stringa, MAX, fp) != NULL){
        sscanf(stringa, "%s %s %f", nome, password, &saldo);
        accountLista = inserimentoTestaAccount(accountLista, nome, password, saldo);
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
abbigliamentoLista *creaNodoAbbigliamento (char *nomeAbbigliamento, float prezzo, int id, int disponibilita){
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
        tmp ->disponibilita = disponibilita;
        /*for(int i = 0; i < TAGLIE; i++)
            tmp ->taglieDisp[i] = rand()%5;*/
    }
    return tmp;
}
abbigliamentoLista *inserimentoCodaAbbigliamento (abbigliamentoLista *abbigliamento, char *nomeAbbigliamento, float prezzo, int id, int disponibilita){
    abbigliamentoLista *tmp = abbigliamento;

    if(tmp == NULL)
        return creaNodoAbbigliamento(nomeAbbigliamento, prezzo, id, disponibilita);
    while(tmp ->next != NULL)
        tmp = tmp ->next;
    tmp -> next = creaNodoAbbigliamento(nomeAbbigliamento, prezzo, id, disponibilita);
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
    int disponibilita;
    int id =0;
    fp = fopen("abiti.txt", "r");
    abbigliamento = freeAbbigl(abbigliamento);
    while(fgets(stringa, 100, fp) != NULL){
        sscanf(stringa, "%f %d %[^\n]s", &prezzo, &disponibilita, nome);
        id++;
        abbigliamento = inserimentoCodaAbbigliamento(abbigliamento, nome, prezzo, id, disponibilita);
    }
    *contatore = id;
    return abbigliamento;
}
void stampaAbbigliamentoLista (abbigliamentoLista *abbigliamento){
    printf("\nStampa della lista abbigliamento\n"); //Debugging purpose
    while(abbigliamento != NULL){
        printf("%d - %s %.2f ",abbigliamento ->id, abbigliamento ->nomeAbbigliamento, abbigliamento ->prezzo);
        if(abbigliamento ->disponibilita >0)
            printf("Disponibile\n");
        else
            printf("Ordinabile\n");
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
scarpeLista *creaNodoScarpe (char *nomeScarpe, float prezzo, int id, int disponibilita){
    scarpeLista *tmp = NULL;
    tmp = malloc (sizeof (scarpeLista));
    if(tmp == NULL)
        return NULL;
    else{
        tmp ->next = NULL;
        allocazioneScarpeLista(nomeScarpe, tmp);
        strcpy(tmp->nomeScarpe,nomeScarpe);
        tmp ->id = id;
        tmp ->disponibilita = disponibilita;
        tmp ->prezzo = prezzo;
    }
    return tmp;
}
scarpeLista *inserimentoCodaScarpe (scarpeLista *scarpe, char *nomeScarpe, float prezzo, int id, int disponibilita){
    scarpeLista *tmp = scarpe;

    if(scarpe == NULL)
        return creaNodoScarpe(nomeScarpe, prezzo, id, disponibilita);
    while(tmp ->next != NULL)
        tmp = tmp ->next;
    tmp -> next = creaNodoScarpe(nomeScarpe, prezzo, id,disponibilita);
    return scarpe;
}
scarpeLista *popolamentoScarpeLista(scarpeLista *scarpe, int contatore){
    FILE *fp = NULL;
    char stringa[100];
    char nome[100];
    int disponibilita;
    float prezzo;
    int id = contatore;
    printf("stampa contatore %d", contatore);
    fp = fopen("scarpe.txt", "r");
    scarpe = freeScarpe(scarpe);
    while(fgets(stringa, 100, fp) != NULL){
        sscanf(stringa, "%f %d %[^\n]s", &prezzo, &disponibilita, nome);
        contatore++;
        scarpe = inserimentoCodaScarpe(scarpe, nome, prezzo, contatore, disponibilita);
    }
    return scarpe;
}
void stampaScarpeLista (scarpeLista *scarpe){
    printf("\nStampa della lista scarpe\n"); //Debugging purpose
    while(scarpe != NULL){
        printf("%d - %s %.2f ",scarpe ->id, scarpe ->nomeScarpe, scarpe ->prezzo);
        if(scarpe ->disponibilita > 0)
            printf("Disponibile\n");
        else
            printf("Ordinabile\n");
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
void stampaAccount(accountUtenteLista *testa){
    while(testa != NULL){
        printf("%s %s %.2f\n", testa ->nomeUtente, testa ->password, testa ->bilancio_conto);
        testa = testa ->next;
    }
}
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
void popolamentoFileAccount(char *nome, char *password, float saldo){
    FILE *fp = NULL;

    fp = fopen("account.txt", "a");
    fprintf(fp, "%s %s %.2f\n", nome, password, saldo);
    fclose(fp);
}
void registrazioneUtente(){

    char nome[MAX], password[MAX];
    float saldo;
    printf("Inserisci il tuo nome\n");
    fgets(nome, MAX, stdin);
    printf("Inserisci la tua password\n");
    fgets(password, MAX, stdin);
    do {
        printf("Inserisci saldo al tuo conto: ");
        scanf("%f", &saldo);
        if(saldo < 0) {
            printf("Il tuo saldo non può essere negativo\nRiprovare\n");
            system("pause");
        }
    }while(saldo < 0);

    nome[strcspn(nome, "\n")] = 0; //Si mangia lo \n della fgets
    password[strcspn(password, "\n")] = 0;
    popolamentoFileAccount(nome, password, saldo);
    printf("\nUtente registrato con successo\nTrasferimento a schermata iniziale in corso...\n");
    system("pause");
}
void gestioneMagazzino(){
    char nomeCapo[MAX];
    float prezzo;
    char nomeScarpe[MAX];
    int disponibilita;
    FILE *abiti = NULL;
    FILE *abitiTaglie = NULL;
    FILE *scarpe = NULL;
    FILE *scarpeTaglie = NULL;

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
            printf("Inserisci quantita: ");
            scanf("%d", &disponibilita);
            fprintf(abiti, "%.2f %d %s", prezzo, disponibilita, nomeCapo);
            fclose(abiti);
        }
        if(scelta == 2) {
            scarpe = fopen("scarpe.txt", "a");
            printf("Inserisci il nome delle scarpe\n");
            fgets(nomeScarpe, MAX, stdin);
            printf("Inserisci il prezzo\n");
            scanf("%f", &prezzo);
            printf("Inserisci quantita: ");
            scanf("%d", &disponibilita);
            fprintf(scarpe, "%.2f %d %s", prezzo, disponibilita, nomeScarpe);
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
void aggiornaAccountDB(accountUtenteLista *accountLista){
    accountUtenteLista *tmp = accountLista;
    FILE *fp = NULL;

    fp = fopen("account.txt", "w");
    while(tmp != NULL){
        fprintf(fp, "%s %s %.2f\n", tmp ->nomeUtente, tmp ->password, tmp ->bilancio_conto);
        tmp = tmp ->next;
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
void sceltaArticoli(abbigliamentoLista *abbigliamento, scarpeLista *scarpe, int contatore, accountUtenteLista *utenteLoggato){
    int scelta;
    abbigliamentoLista *tmpAbbigliamento = abbigliamento;
    scarpeLista *tmpScarpe = scarpe;

    printf("Scelta:");
    scanf("%d", &scelta);

    if(scelta <= contatore) {
        while (tmpAbbigliamento != NULL) {
            if (scelta == tmpAbbigliamento->id) {
                if(utenteLoggato ->bilancio_conto >= tmpAbbigliamento ->prezzo) {
                    printf("Hai acquistato %s", tmpAbbigliamento->nomeAbbigliamento);
                    utenteLoggato ->bilancio_conto -= tmpAbbigliamento ->prezzo;
                }
            }
            tmpAbbigliamento = tmpAbbigliamento->next;
        }
    }else{
        while(tmpScarpe != NULL){
            if(scelta == tmpScarpe ->id) {
                if(utenteLoggato ->bilancio_conto >= tmpScarpe ->prezzo) {
                    printf("Hai acquistato %s", tmpScarpe->nomeScarpe);
                    utenteLoggato->bilancio_conto -= tmpScarpe->prezzo;
                }
            }
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
                sceltaArticoli(abbigliamento, scarpe, contatore, utenteLoggato); //INSERIRE FUNZIONE DI ACQUISTO ARTICOLO
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
                aggiornaAccountDB(accountLista);
                stampaAccount(accountLista);
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
