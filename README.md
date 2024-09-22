# MHD_GW_ESP32

  ---- Old version. Please use rather MHD_GW_WWW ----

ESP32 utility to extract Prague public transport departures and use it in your projects, like "zivy obraz"
The rest is in Czec, as it is local content, specific to Prague transport system.

Jedná se o "proof of concept". Rychlý bastl.
Budu rád, když to někdo uchopí a poběží dál.

Primární důvod je mít na Živém obrazu odjezdy ze zastávek MHD a aktuální informace. Kód je okomentovaný a připojuji trochu teorie v dokumentaci. Tamtéž jsou screenshoty z toho, jak mám nastaven Živý obraz pro tuto konkrétní obrazovku.

NEW: verze 05:
- zobrazuje hvězdičku před číslem linky, pokud je spoj klimatizován
- v dotazu na Golemio je časové okno na dotaz rozšířeno na 6h, aby se před půlnocí zobrazily časné ranní spoje
- mírně vylepšeno zacházení s chybami
- komfortnější nastavování zastávek a parametrů - přesunuto do setup.h
- volba, zda chceme dynamickou či fixní IP

## Rychlý start:
- Přečtěte si dokumentaci (...já vím :)
- Najděte si kódy zastávek, o které se zajímáte, případně udělejte první pokus s těmi vyplněnými
- získejte API token a zkopírujte ho do setup.h
- pokud nepotřebujete 3 zastávky, ale třeba jen jednu, upravte setup.h
- nastavte si WiFi údaje
- doplňte token pro živý obraz
- najděte v šupleti nějaké ESP32
- nahrajte program a spusťte to se zapnutou konzolí
- pokud budete dostávat všude odpovědi "200", jděte do živého obrazu a najdete tam data pro vaši obrazovku
- bavte se a experimentujte
- ....jo a chovejme se na Golemio serveru slušně, ať nám nezabouchnou dveře, je to super, že jsou data takto k dispozici!

  (nejsem programátor, tak buďte shovívaví)

  # Ostatní města?
  Nevím. Snažil jsem se najít Brno a Plzeň a pak jsem toho nechal, podle dostupných aplikací ta data existují, ale jsou za zdí. Pokud někdo najdete někde něco, ozvěte se, rád budu spolupracovat. Pokud něco objevím, bude to tady.
  
