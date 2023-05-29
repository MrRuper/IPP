Zadanie gra, część 2
Jako drugą część zadania należy zaimplementować interfejs gry umożliwiający przeprowadzanie rozgrywki i korzystający z modułu silnika zaimplementowanego w części pierwszej. Ponadto w drugiej części zadania należy poprawić błędy z pierwszej części zadania.

Uruchamienie programu
Program uruchamia się w konsoli Linuksa poleceniem

./game width height players areas
gdzie parametry programu są parametrami wywołania funkcji game_new. Program powinien dokładnie sprawdzać poprawność podanych mu parametrów.

Kończenie programu
Zasadniczo program powinien zakończyć się kodem 0, chyba że wystąpił jakiś krytyczny błąd – wtedy program powinien wypisać stosowny komunikat na standardowe wyjście diagnostyczne i zakończyć się kodem 1. Przed zakończeniem działania program powinien jawnie zwolnić całą alokowaną pamięć, w szczególności powinien wywołać funkcję game_delete.

Interaktywny tryb tekstowy
Program gry działa w trybie tekstowym w konsoli Linuksa. Program wyświetla planszę, a pod planszą wiersz zachęcający gracza do wykonania ruchu. Program prosi o wykonanie ruchu kolejnych graczy, przy czym pomija graczy, dla których funkcja game_free_fields zwróciła 0. Ruch wykonuje się, przesuwając kursor na wybrane pole za pomocą klawiszy ze strzałkami, a następnie wciskając klawisz spacja, aby postawić pionek na wybranym polu. Gracz może zrezygnować z ruchu, wciskając klawisz C. Gra kończy się, gdy już żaden gracz nie może wykonać ruchu lub po jednokrotnym wciśnięciu kombinacji klawiszy Ctrl-D. Wtedy program wypisuje ostateczną planszę gry wraz z podsumowaniem, ile pól zajął każdy z graczy.

Wciśnięcie klawiszy literowych powinno być rozpoznawane niezależnie od wciśnięcie klawisza Shift i stanu CapsLock. Program powinien uniemożliwiać użytkownikom wykonywanie błędnych akcji. Program powinien zakończyć się, wypisując stosowny komunikat o błędzie, jeśli plansza nie mieści się w oknie terminala.

Wymagania
Rozwiązanie drugiej części zadania powinno korzystać z własnego, ewentualnie samodzielnie zmodyfikowanego, rozwiązania części pierwszej. Wolno rozbudować moduł silnika gry o potrzebne funkcja, ale nie wolno modyfikować działania funkcji zdefiniowanych w pierwszej części zadania.

Jako rozwiązanie należy dostarczyć:

pliki *.c i *.h z implementacją gry,
skrypt makefile kompilujący program.
Zaleca się kompilowanie za pomocą polecenia gcc z parametrami

-Wall -Wextra -Wno-implicit-fallthrough -std=c17 -O2
Rozwiązanie powinno się kompilować i uruchamiać w laboratorium studenckim i na maszynie students.

Polecenie make clean powinno usuwać wszystkie pliki stworzone poleceniem make.

Zawartość plików game_example.c i game_test_1.c nie będzie oceniana – można w nich umieścić własne testy.

UWAGA 1: funkcja main programu musi znajdować się w pliku game_main.c.

UWAGA 2: w wyniku kompilacji powinien powstać plik wykonywalny game.
