# WordleGame<br>
## Progetto relativo all'insegnamento "Algoritmi e Principi dell'Informatica" di Ingegneria Informatica al Politecnico di Milano. 2023-2024<br>

Si chiede di implementare una versione avanzata del noto gioco Wordle. Di seguito le regole.
<br><br>
WordChecker<br>
 * Un sistema che, al suo cuore, controlla la corrispondenza tra le lettere di 2 parole di ugual lunghezza. <br>
 * Le parole sono intese come sequenze di simboli che possono essere caratteri alfabetici minuscoli (a-z) o maiuscoli (A-Z), cifre numeriche (0-9), oppure i simboli - (trattino) e _ ("underscore")<br>
 * esempio di parola di 20 simboli: djHD1af9fj7g__l-ssOP<br>
 * Il sistema legge da standard input una sequenza di informazioni e istruzioni, e produce delle stringhe in output a seconda dei casi.<br>
 * Più precisamente, il sistema legge:<br>
	* un valore k, che indica la lunghezza delle parole<br>
 	* una sequenza (di lunghezza arbitraria) di parole, ognuna di lunghezza k, che costituisce l'insieme delle parole ammissibili<br>
		* si dia pure per scontato che la sequenza di parole non contenga duplicati<br>
 * A quel punto, viene letta da standard input una sequenza di "partite", in cui l'inizio di ogni nuova partita è marcato dal comando (letto sempre da input) +nuova_partita<br>


* Le sequenze di stringhe in input per ogni partita (successive al comando +nuova_partita) sono fatte nel seguente modo:<br>
	* parola di riferimento (di lunghezza k caratteri)<br>	
		 * si assuma che la parola di riferimento appartenga all'insieme di parole ammissibili<br>
 	* numero nmassimo di parole da confrontare con la parola di riferimento<br>
 	* sequenza di parole (ognuna di k caratteri) da confrontare con la parola di riferimento<br>
 * Ogni tanto, nella sequenza di stringhe in input, può comparire il comando +stampa_filtrate, il cui effetto è spiegato in seguito<br>
 * Inoltre, sia durante una partita, che tra una partita e l'altra, possono comparire i comandi +inserisci_inizio e +inserisci_fine che racchiudono tra di loro una sequenza di nuove parole da aggiungere all'insieme delle parole ammissibili<br>
 	* le parole aggiunte sono anch'esse di lunghezza k, e si dà sempre per scontato che non ci siano parole duplicate (neanche rispetto alle parole già presenti nell'insieme di quelle ammissibili)<br>


 * Per ogni parola letta (che nel seguito indichiamo con p), da confrontare con la parola di riferimento (che nel seguito indichiamo con r), il programma scrive su standard output una sequenza di k caratteri fatta nella seguente maniera.<br>
 	* nel seguito, indichiamo con p[1], p[2], … p[k] i caratteri della parola p, con r[1], r[2], … r[k] quelli della parola r, e con res[1], res[2], … res[k] quelli della sequenza stampata<br>
 * Per ogni 1 ≤ i ≤ k, si ha che<br>
 	* res[i] è il carattere + se l'i-esimo carattere di p è uguale all'i-esimo carattere di r<br>
 		* cioè se vale che p[i] = r[i], quindi p[i] è "in posizione corretta"<br>
 	* res[i] è il carattere / se l'i-esimo carattere di p non compare da nessuna parte in r<br>
 	* res[i] è il carattere | se l'i-esimo carattere di p (indicato nel seguito come p[i]) compare in r, ma non in posizione i-esima; tuttavia, se in r compaiono ni istanze di p[i], se ci è il numero di istanze del carattere p[i] che sono in posizione corretta (chiaramente deve valere che ci ≤ ni) e se ci sono prima del carattere i-esimo in p almeno ni-ci caratteri uguali a p[i] che sono in posizione scorretta, allora res[i] deve essere / invece che |<br>


 * Per esempio: <br>
se r = abcabcabcabcabc (quindi r[1] = a, r[2] = b, ecc.) e <br>
p = bbaabccbccbcabc (con p[1] = b, p[2] = b, …), <br>
abbiamo che res risulta la seguente sequenza: <br>
abcabcabcabcabc<br>
 bbaabccbccbcabc<br>
 /+|+++|++/+++++<br>
 * si noti che res[1] = / perché in r ci sono solo 5 b, p ne ha 6, e tutte le b successive a p[1] sono nel posto corretto<br>
 * similmente, res[10] = / perché r ha 5 c, p ne ha 6, di cui 4 al posto giusto, e c'è già una c prima di p[10] (in p[7]) che è al posto sbagliato<br>

 
* Altri esempi di confronti (dove la prima riga è la parola di riferimento r, la seconda è p, e la terza è l'output res)<br>
 * djPDi939-s__e-s<br>
 gioSON-we2_w234<br>
 /|////|/|/+//|/<br>
 * djPDi939-s__e-s<br>
 kiidsa92KFaa94<br>
/|/||/|/////|/|<br>
 * djPDi939-s__e-s<br>
 ewi-n4wp-sesr-v<br>
 |/|/////++/|/+/<br>
 * DIk834k249kaoe_<br>
 48kDkkkf-saancd<br>
 ||+||/+////+///<br>


 * Se da standard input viene letta una parola che non appartiene all'insieme di quelle ammissibili, il programma scrive su standard output la stringa not_exists<br>
 * Se invece viene letta la parola r (cioè se p = r), allora il programma scrive ok (senza stampare il risultato dettagliato del confronto) e la partita termina<br>
 * Se, dopo avere letto n parole ammissibili (con n, si ricordi, numero massimo di parole da confrontare con r), nessuna di queste era uguale a r, il programma scrive ko (dopo avere stampato il risultato del confronto dell'ultima parola), e la partita termina<br>
 * Dopo che la partita è finita:<br>
 	* Non ci possono essere altre parole da confrontare (ma ci potrebbe essere l'inserimento di nuove parole ammissibili)<br>
 	* Se in input c'è il comando +nuova_partita, ha inizio una nuova partita<br>
 * Ogni confronto tra p e r produce dei vincoli appresi dal confronto<br>
 * Per esempio, dal seguente confronto<br>
 abcabcabcabcabc<br>
 bbaabccbccbcabc<br>
 /+|+++|++/+++++<br>
 si apprende che b è in posizioni 2, 5, 8, 11, 14, che ci sono solo 5 b in r (la sesta b dà luogo a /), che c è in posizioni 6, 9, 12, 15, che non è in posizioni 7 e 10, che ci sono solo 5 c (come prima, la sesta dà luogo a /), che a è in posizioni 4 e 13, ma non è in posizione 3<br>
 * In maniera analoga, dal seguente confronto<br>
 djPDi939-s__e-s<br>
 gioSON-we2_w234<br>
 /|////|/|/+//|/<br>
 si apprende che in r non ci sono g, né o, né S, che in r c'è almeno una i e che questa non è in posizione i, che c'è almeno un - e che non è in posizione 7, ecc.<br>
 * Quando, durante una partita, da input si legge il comando +stampa_filtrate, il programma deve produrre in output, in ordine lessicografico, l'insieme delle parole ammissibili che sono compatibili con i vincoli appresi fino a quel momento nella partita, scritte una per riga<br>
 * si noti che i vincoli appresi riguardano, per ogni simbolo:<br>
 	1. se il simbolo non appartiene a r<br>
	2. posti in cui quel simbolo deve comparire in r<br>
 	3. posti in cui quel simbolo non può comparire in r<br>
 	4. numero minimo di volte che il simbolo compare in r<br>
 	5. numero esatto di volte che il simbolo compare in r<br>
 		* si noti che il vincolo 5 è più forte del vincolo 4<br>
 * l'ordine dei simboli (usato per stabilire l'ordine lessicografico delle parole) è quello specificato dallo standard ASCII<br>
 * Inoltre, dopo ogni confronto, il programma deve stampare in output il numero di parole ammissibili ancora compatibili con i vincoli appresi (tranne nel caso in cui l'esito del 
confronto sia not_exists)<br>
<br>
Un'esecuzione d'esempio<br>
 Input ricevuto<br>
 5<br>
 8adfs<br>
 5sjaH<br>
 KS06l<br>
 Hi23a<br>
 laj74-s9k0<br>
 sm_ks<br>
 okauE<br>
 +nuova_partita<br>
 5sjaH<br>
 4<br>
 KS06l<br>
 had7s<br>
 okauE<br>
<br>
 Output (su 2 righe, in colonna): /////, 5<br>
 Output: not_exists<br>
 Output: //|//, 3<br>
<br>
Un'esecuzione d'esempio<br>
 Input ricevuto<br>
 +stampa_filtrate<br>
 +inserisci_inizio<br>
 PsjW5<br>
 asHdd<br>
 paF7s<br>
 +inserisci_fine-s9k0<br>
 sghks<br>
 +stampa_filtrate<br>
 sm_ks<br>
 +inserisci_inizio<br>
 _fah<br>
0D7dj<br>
 +inserisci_fine<br>
<br>
 Commenti e Output Atteso<br>
 Output (in colonna): 5sjaH, 8adfs, Hi23a<br>
 Nuove parole da aggiungere a quelle ammissibili (e quelle che sono compatibili  con i vincoli appresi sono da aggiungere all'insieme delle parole compatibili
 con i vincoli appresi)<br>
 Output: /+///, 2<br>
 Output: not_exists<br>
 Output (in colonna): 5sjaH, asHdd<br>
 Output: |////, 2<br>
 Output subito dopo mappa confronto (raggiunto numero massimo parole): ko<br>
 Nuove parole da aggiungere a quelle ammissibili<br>
<br>
Un'esecuzione d'esempio<br>
 Input ricevuto<br>
 +nuova_partita<br>
 okauE<br>
 3<br>
 laj74<br>
 +stampa_filtrate<br>
 sm_ks<br>
 okauE<br>
<br>
 Commenti e Output Atteso<br>
 Inizio nuova partita<br>
 Parola di riferimento<br>
 numero massimo di parole da confrontare in questa partita<br>
 Output: /|///, 4<br>
 Output (in colonna): Hi23a , _fah-, asHdd , okauE<br>
 Output: ///|/, 1 <br>
Output: ok<br>
