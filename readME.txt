I.Biblioteci

Pentru acest proiect nu este nevoie de instalarea unor biblioteci.


II.Compilarea

In fisierul in care se afla proiectul am un Makefile, pe care il folosim in modul urmator:
- comanda "make all" va compilta cele 2 programe: serverul, respectiv clientul;
- comanda "make sv" va executa binarul pentru server si-l va rula;
- iar comanda "make cl" va executa binarul pentru client unde este specificata adresa si portul, astfel, acesta conectandu-se la server.


III.Interactiunea cu programul
Avem doua cazuri:

a)Din moment ce un client s-a conectat la user, acesta va avea 2 comenzi disponibile:
- comanda "quit" pentru user-ul nelogat care vrea sa iasa din server;
- si comanda "login [insert password]" in care inseram o parola si avem un exemplu mai jos:
exemplu: login jom

b)In cazul in care logarea a fost un succes, clientul va avea la dispozitie 5 comenzi:
- comanda "show_categories" in care ii va fi aratat continutul fisierului de categorii;
- comanda "create_category [insert category]", care ii va permite clientului sa creeze o categorie care-si va da append in fisierul de categorii, iar apoi va fi creat un fisier .txt cu numele categoriei introduse de catre client;
exemplu: create_category banking

- comanda "write_in_category [insert category] [insert password,title,username,url,notes]", aceasta comanda ii va folosi clientului in a scrie informatii in categoria selectata de acesta, iar apoi se introduc informatiile, care sunt de preferat separate printr-o virgula, a nu se folosi spatii cand se scriu informatii precum parola,titlu,url,username si notite, intrucat daca este apasat spatiul in sectiunea de introducere a informatiilor, datele urmatoare scrise in terminal nu vor fi introduse;
exemplu: write_in_category banking bcr,terry,url,pin:0000

- comanda "show_category [insert category]", aceasta comanda ii permite clientului, sa vada ce continut se afla in categoria data, ca si exemplu de comanda ar fi:
exemplu: show_category banking

-comanda "quit", care il va deconecta pe clientul logat de la server.
