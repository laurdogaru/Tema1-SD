Tema 1
-Am structurat programul astfel incat fiecare caracter dintr-un text sa fie 
un nod in lista "cuvant", care este un nod in lista "linie", care este un
nod in lista "text"

-Am implementat pentru fiecare nivel numit mai sus functii de adaugare, 
stergere, stergere la o anumita pozitie

-Am folosit doua stive(undo si redo) pe care le-am declarat global pentru
stocarea comenzilor.Stivele au camp de date de tip sir de caractere;

-Am facut functii de adaugare si pop pentru stive

-Am implementat fiecare functie numita in enuntul temei(backspace,
delete_line, go_to_line, go_to_char, delete_char, delete_word, 
delete_all_words, replace_word, replace_all_words).Fiecare functie de acest 
tip primeste ca parametri textul asupra caruia se lucreaza si
pozitia cursorului din acel moment.

-Pentru a implementa functiile delete_word si replace_word am facut o functie
numita "strlist", care primeste ca parametri textul asupra caruia se lucreaza,
pozitia la acel moment si cuvantul cautat, si intoarce prima pozitie de dupa 
cursor a cuvantului cautat, daca acesta exista in text

-Initial, tot textul din fisierul de intrare este pus intr-o lista de tip
"text" prin comanda "citire".

-Dupa, prin functia "extragere_comenzi", comenzie sunt puse intr-un vector
de siruri de caractere

-Functia "interpretare_comanda" "traduce" sirurile de caractere, asociind
un sir de caractere cu o functie de prelucrare a textului

-Functia "editor" primeste ca argumente textul initial citit si vectorul de
siruri de caractere in care sunt stocate comenzile,parcurge textul initial si
vectorul de comenzi, si intoarce textul modificat.

-In functia editor,se contorizeaza numarul de aparitii al separatorului "::i".
In functie de paritatea contorului,se adauga linii(la sfarsit sau la pozitia 
cursorului) in textul ce va fi returnat sau se efectueaza comenzi.

-In cazurile in care se adauga text sau se efectueaza comenzi normale(excluzand
undo, redo si save ) se adauga in stiva de undo "added text", respectiv
comanda efectuata sub forma de sir de caractere

-In caz ca se primeste comanda "undo", se da pop stivei de undo, iar
rezultatul se pune in varful stivei de redo

-In caz ca se primeste comanda "redo", se da pop stivei de redo, iar
rezultatul se pune in varful stivei de undo

-La fiecare adaugare de text nou, se contorizeaza cu variabila "k" cate linii
au fost introduse. Astfel daca se da "undo" la adaugare de text, se vor sterge
ultimele k linii din text

-Daca urmatoarea comanda dupa "undo" pentru adaugare de text este "redo",
parcurgerea textului initial si parcurgerea vectorului de comenzi sunt 
intoarse pana la punctul in care s-a  adaugat ultimul fragment de text. De 
asemenea, se scot din vectorul de siruri de caractere comenzile "u" si "r".
Dupa, inserarea si efectuarea comenzilor se desfasoara in mod normal.

