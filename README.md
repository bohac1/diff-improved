# Rozšířený porovnávač
Autor: Michal Boháč
## Funkcionalita
### Porovnávání obrázků ve formátu BMP
- možnost práce s obrázky je omezena pouze na obrázky 24 a 32 bitové hloubky barev ve formátech BGR a BGRA, dále nebude možnost pracovat s obrázky, které používají kompresi standardu BMP
- pracovní módy:
    - možnost zprůměrování barev dvou obrázků
    - možnost odečíst jeden obrázek od druhého
- výstupem je obrázek opět ve formátu bmp, který bude vypsán na standardní výstup, nebo do souboru

### Porovnávání bitových souborů
- možnosti výpisu na standardní výstup:
    - výpis v dekadické formě
    - výpis v hexadecimální formě
- výstupem je výpis binárního souboru na standardní výstup, nebo do souboru
- výstupem je odečtení druhého souboru od prvního po bajtech

### Porovnávání textových souborů
- možnost vynutit bitové porovnávání
- porovnávání probíhá v rámci řádků
- výstupem je výpis na standardní výstup, nebo do souboru vyznačující rozdílné řádky

### Porovnávání složek
- pracovní módy:
    - možnost porovnávat pouze strukturu složek
    - možnost porovnávat strukturu složek i s jejich soubory
    - možnost nastavit hloubku prohledávání pro porovnávání
    - možnost vypnutí porovnávání skrytých souborů a složek (soubory a složky začínající tečkou) ve složkách 
- výstupem je výpis na standardní výstup (,nebo do textového souboru) vyobrazující rozdíly ve struktuře složek popřípadě rozdíly v jejich souborech 

Rozhraní volání programu je popsáno v souboru flags.txt .
Využití polymorfismu bude v ruzných druzích objektů zděděných z objektu DiffObj. Toto bude například využito v porovnávání dvou DiffObj obejktů, v ukládání DiffObj objektů do vnitřní struktury objektu FolderObj, načítání a vypisovnání DiffObj objektu.
