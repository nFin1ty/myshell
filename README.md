# myshell
Εργασία Λειτουργικών Συστημάτων 2018  
Γαλάζιος Ευστάθιος (8614)

Το πρόγραμμα εκτελείται με ```./myshell``` για λειτουργία εισόδου εντολών από τον χρήστη ή ```./myshell <batchfile>``` για λειτουργία εκτέλεσης δέσμης εντολών από αρχείο.

## Επεξήγηση κυρίως προγράμματος
Με την εκτέλεση του προγράμματος ελέγχεται ο αριθμός των ορισμάτων εισόδου και εάν είναι μεγαλύτερος του 1, ορίζεται το *batchMode*, αφού αυτό σημαίνει ότι ο χρήστης έχει προσδιορίσει επιπλέον ως όρισμα το όνομα του αρχείου που περιέχει τη δέσμη εντολών προς εκτέλεση. Αμέσως μετά γίνεται άνοιγμα αυτού του αρχείου για ανάγνωση. 

Στη συνέχεια, γίνεται ανάγνωση της εισόδου, ανάλογα με τη λειτουργία προγράμματος. Σε λειτουργία *batchMode*, ως είσοδος διαβάζεται μια γραμμή του αρχείου, ενώ σε λειτουργία *interactive* η είσοδος δίνεται από τον χρήστη, αφού πρώτα τυπώνεται στο τερματικό η συμβολοσειρά της μορφής *επώνυμο_ΑΕΜ*. Εάν δεν διαβαστεί επιτυχώς γραμμή από το αρχείο (π.χ. εάν το πρόγραμμα φτάσει στο τέλος του αρχείου και δεν περιέχει την εντολή `quit`), το πρόγραμμα τερματίζεται. Οι κενές γραμμές του αρχείου παραλείπονται.

Η συνέχεια είναι κοινή και για τις 2 λειτουργίες. Πρέπει, αρχικά, να βρεθεί ο delimiter που χωρίζει τις εντολές (εδώ έγινε η παραδοχή ότι σε κάθε γραμμή μπορεί να υπάρχει μόνο ένας από τους delimiter ; ή &&). Εάν η είσοδος αποτελείται από μόνο μία εντολή, επιστρέφεται ο χαρακτήρας κενού ως ένδειξη μη ύπαρξης delimiter. Η αναζήτηση αυτή θα μας επιστρέψει μόνο εάν υπάρχει «;» ή «&» στη γραμμή. Επειδή όμως θέλουμε να υπάρχουν ακριβώς 2 «&» μεταξύ των εντολών όταν χρησιμοποιείται το & ως delimiter και η `strtok()` δεν μπορεί να το αναγνωρίσει αυτό, τον έλεγχο εγκυρότητας των delimiters αναλαμβάνει η συνάρτηση `verifyDelims()` που ενημερώνει για την ορθότητα της εισόδου ως προς τη χρήση των delimiters. Εάν βρεθεί λάθος, το πρόγραμμα τερματίζεται με ενημερωτικό μήνυμα ως προς τον σωστό διαχωρισμό των εντολών. Εάν η είσοδος είναι σωστή, το πρόγραμμα συνεχίζει με τον διαχωρισμό των ξεχωριστών εντολών. Για αυτό χρησιμοποιείται η συνάρτηση `parseCommands()` η οποία αποθηκεύει ξεχωριστά κάθε εντολή στο array *commands* και επίσης θέτει στη μεταβλητή *commandsCount* την τιμή του πλήθους των εντολών. Αυτό γίνεται «σπάζοντας» την είσοδο στα σημεία των delimiter που χρησιμοποιούνται, εκτός αν αποτελείται μόνο από μία εντολή (στο συγκεκριμένο πρόγραμμα αυτό κωδικοποιείται με τον χαρακτήρα κενού ως delimiter), οπότε τότε αποθηκεύεται αυτούσια στο array και ο αριθμός των εντολών τίθεται 1.

Ακολουθεί ένας βρόχος στον οποίο γίνεται η εκτέλεση των εντολών. Κάθε επανάληψη του βρόχου είναι υπεύθυνη για την εκτέλεση μίας εντολής, μέσω της θυγατρικής διεργασίας που θα δημιουργήσει. Αρχικά γίνεται fork και ελέγχεται από τη θυγατρική διεργασία εάν η τρέχουσα εντολή είναι η ειδική εντολή `quit` με την οποία πρέπει να τερματίζεται το πρόγραμμά μας. Ο τερματισμός πρέπει να γίνει από τη γονική διεργασία, οπότε η θυγατρική τερματίζεται με τον κωδικό 7, για ενημέρωση της γονικής για τερματισμό λόγω `quit`. Σε διαφορετική περίπτωση, θα πρέπει να χωριστούν τα ορίσματα της εντολής σε ένα array το οποίο θα δοθεί ως όρισμα στην `execvp()`. Αυτό γίνεται με τη συνάρτηση `parseArgs()`, η οποία χωρίζει μια εντολή στα ορίσματά της (το πρώτο όρισμα θα είναι το όνομα του εκτελέσιμου) και τα τοποθετεί στο array *args*. Επίσης, θέτει *NULL* το τελευταίο στοιχείο του array, όπως απαιτεί η `execvp()` ώστε να γνωρίζει πού σταματούν τα ορίσματα. Σε περίπτωση αποτυχίας της, εμφανίζεται μήνυμα σφάλματος και η θυγατρική διεργασία τερματίζεται με κωδικό 2.

Η γονική διεργασία περιμένει να τερματιστεί η θυγατρική και, αφού τερματιστεί, ελέγχει τον κωδικό με τον οποίο τερματίστηκε. Εάν αυτός είναι 7, το πρόγραμμα τερματίζεται (αφού αυτό σημαίνει ότι «εκτελέστηκε» η εντολή `quit`), ενώ αν είναι 2, αυτό σημαίνει ότι υπήρχε σφάλμα στην εκτέλεση της εντολής. Εάν ο delimiter στη συγκεκριμένη γραμμή ήταν ο «&&», τότε θα πρέπει να μην εκτελεστούν οι επόμενες εντολές, όπως ορίζεται στις προδιαγραφές της άσκησης, οπότε και γίνεται break στον βρόχο και συνεχίζει η ανάγνωση της επόμενης γραμμής εισόδου (είτε από τον χρήστη, είτε από το αρχείο, ανάλογα με τη λειτουργία που έχει οριστεί αρχικά). Εάν ο delimiter ήταν ο «;», τότε απλά συνεχίζει με την εκτέλεση της επόμενης εντολής.

## Επεξήγηση συναρτήσεων
**char findDelim(char \*input)**

Χρησιμοποιείται για να βρεθεί ο delimiter που υπάρχει στην είσοδο. Αναζητά για τον χαρακτήρα «;» ή «&», τον οποίο και επιστρέφει. Σε διαφορετική περίπτωση επιστρέφει τον χαρακτήρα του κενού (ένδειξη ότι δεν υπάρχει delimiter, δηλ. υπάρχει μόνο 1 εντολή). Φυσικά, το σκέτο «&» δεν είναι έγκυρος delimiter, αλλά αυτός ο έλεγχος θα γίνει σε ξεχωριστή συνάρτηση.

**int verifyDelims(char \*input, char \*delimiter)**

Εδώ γίνεται ο έλεγχος της εγκυρότητας των delimiter που χρησιμοποιήθηκαν. Στην περίπτωση του χαρακτήρα «;» δεν γίνεται κάποιος έλεγχος, αφού αν υπάρχει παραπάνω από 1 «;» μπορεί να θεωρηθεί ως ύπαρξη κενών εντολών μεταξύ διαδοχικών «;», ενώ για τον «&» θα πρέπει να εμφανίζεται ακριβώς 2 φορές. Επιστρέφει 1 ή 0, ανάλογα με την ορθότητα της εισόδου.

**void parseCommands(char \*input, char \*\*commands, char \*delimiter, int \*commandsCount)**

Αρχικά αφαιρείται από τη γραμμή το \n ή το \r που θα περιέχει στο τέλος. Έπειτα, η γραμμή «σπάει» στις εντολές (μαζί με τα ορίσματά τους) από τις οποίες αποτελείται. Για το σημείο όπου θα χωριστεί, χρησιμοποιείται ο χαρακτήρας delimiter που έχει βρεθεί προηγουμένως και έχει δοθεί ως όρισμα κατά την κλήση της συνάρτησης. Με διαδοχικές κλήσεις της strtok, κάθε εντολή αποθηκεύεται τελικά στο array *commands*, και η συνάρτηση αποθηκεύει στο *commandsCount* τον αριθμό εντολών που διάβασε.

**void parseArgs(char \*command, char \*\*args)**

Χωρίζει μια εντολή στα ορίσματά της, τα οποία τοποθετεί στο array *args*. Σαν πρώτο όρισμα τοποθετείται το όνομα του εκτελέσιμου αρχείου. Ως τελευταίο όρισμα η σταθερά *NULL* που σηματοδοτεί το τέλος των ορισμάτων, όπως χρειάζεται για τη συνάρτηση `execvp()`.
