# RobotSwarmAPI
API for a swarm of robots.

## Sketch in polish

- Łączność: 
Tutaj najprawdopodobniej zostanie użyty WiLink8 dostarczany przez Texas Instruments (BB natywnie wspiera to oprogramowanie). Na jego bazie postawimy wifi-mesh, gdzie każdy robot będzie równorzędnym węzłem sieci. Opis wifi-mesh na podstawie WiLink8: http://www.ti.com/lit/an/swaa166/swaa166.pdf

- Komunikacja: 
Na każdym BB będzie pracować serwer i klient websocketów (z biblioteki Qt5), który będzie komunikował się z warstwą softu komunikującą się z API (np. obsługa odbioru odczytów z czujników innych robotów roju).

- Struktura API: 
API będzie udostępniać metody, sygnały i sloty (zapewniane przez Qt). Dzięki temu programowanie zadań roju będzie polegało na odpowiednim łączeniu sygnałów (takich jak: odebranie paczki odczytów z czujników całego roju, odebranie rozkazu do przemieszczenia się etc.) ze slotami (np. wyrusz do stacji ładowania, pojedź o x metrów naprzód, bądź też slotami samego programisty, którymi mogą być całe algorytmy).
Mogą być bardzo precyzyjne i drobnostkowe (takie jak choćby zmiana napięcia baterii o 0,1V), a ich ilość będzie ograniczać wydajność płytki oraz zachowanie API jako "user-friendly". Możliwe jest np. podzielenie API na kilka warstw dziedziczących po sobie ze zestopniowanym poziomem szczegółowości i złożoności i pozostawienie woli programisty, która wersja bedzie dla niego odpowiednia.

- Komunikacja z hardwarem: 
Komunikacja z hardwarem będzie wykorzystywać jeszcze jedną warstwę - kod w C zarządzający najniższymi akcjami (ruchem, odbiorem odczytów etc.). To ta warstwa ma być docelowo wymienialna przy prowadzeniu symulacji.
