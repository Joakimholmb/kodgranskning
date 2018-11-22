# Inlupp 1

make freq-count #Bygg programmet

./freq-count FIL.txt #Kör programmet

make test #Bygg testfiler

make run #Kör testfil


Error-hantering:
I hash_table_lookup returneras en strukt option_t, som returnernar en bool och ett värde om bool är sann. Om boolen är falsk säger vi att värdet ej är definierat.

Errors hanteras annars i errno.h. I den här modulen används enbart EINVAL i ett fåtal fall. Om fel argument skickas in så returneras 0 på formen elem_t



Designval:

Det är upp till användaren att börja programmet med create (för hash_table, list_linked och iterator) och avsluta med destroy för att undvika minnesläckor. Beteendet om användaren kör funktioner utan att första köra create är odefinierat.

Om du kallar hash_table_keys och hash_table_values på samma hashtabell kommer de länkade listorna keys och values motsvara en unik entry i hashtabellen på varje index.


Coverage:

File 'hash_test.c'
Lines executed:82.19% of 292

File 'list_test.c'
Lines executed:85.89% of 319

