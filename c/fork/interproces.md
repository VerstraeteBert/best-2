# Inter-proces comm  

UNIDIRECTIONELE COMMUNICATIE
vb cat test.xd | wc -l
pipe(fds) -> fds: int[2]
fds[0] => read (cat ..)
fds[1] => write (wc -l)

einden sluiten die niet nodig zijn;
enkel lezen -> close(fds[0])
enkel schrijven -> close(fds[1])

Om in twee richtingen te communiceren -> 2 pipes nodig!

Indien communicatie niet meer nodig is -> zorg dat alle kanalen gesloten zijn :-)

1) fork -> pipe  
 (losstaande) pipes gecreeërd in beide forks, vaak niet wenselijk

2) pipe -> fork  
pipe wordt gedeeld tussen kind- en ouderprocessen  
communicatie tussen kind en ouder mogelijk