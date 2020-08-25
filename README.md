stejné jako zadani.txt
# Rozšířený porovnávač
Autor: Michal Boháč
## Zadání z progtestu
Rozšířený porovnávač

Vytvořte program, který bude obdobou UNIXového přikazu diff. Implementujte jej s následujícími vlastnostmi:

- bude schopen přizpůsobovat svůj výstup na základě typů vstupních souborů
- lze si vynutit druh výstupu (můžu HTML soubor porovnávat binárně)
- podporuje alespoň 3 různé druhy souborů
- umožňuje porovnávat celé složky (na obsah složek i obsah souborů)
- pro alespoň 1 druh podporuje alespoň 2 nastavitelné parametry (co se má ignorovat, definovat ekvivalentní výrazy, ...)

Ukázka různých druhů souborů:

- binární soubor - porovnává data na úrovni bytů
- textový soubor - porovnává textovou reprezentaci, ignoruje více mezer, neignoruje odstavce
- HTML stránka - ignoruje pořadí atributů
- JSON - ignoruje pořadí dat
- C zdrojový kód - pokusí se namapovat názvy proměnných a funkcí (např. v závislosti na pořadí jejich volání, definice, počtu použití)
- obrázek - výstupem může být obrázek ukazují průměr

Volitelně můžete porovnávat dva různé druhy souborů na úrovni významové: HTML vs. Markdown, adresář vs. komprimovaný archiv.

Kde lze využít polymorfismus? (doporučené)

- Různé styly výstupu: textový každá změna na řádek, rozdíly přímo zaznamenané do souborů, nový soubor s vyznačenými změnami obou verzí, ...
- Druhy souborů: binární, textový, Markdown, HTML, JSON, XML, zdrojový kód, ...
- Parametry porovnávání: ignorování bílých znaků, ekvivalentní výrazy, ignorování base URL v odkazech, ...

## Upřesnění zadání
### Porovnávání obrázků ve formátu BMP
- možnost práce s obrázky bude omezena pouze na obrázky 24 a 32 bitové hloubky barev ve formátech BGR a BGRA, dále nebude možnost pracovat s obrázky, které používají kompresi standardu BMP
- pracovní módy:
    - možnost zprůměrování barev dvou obrázků
    - možnost odečíst jeden obrázek od druhého
- výstupem bude obrázek opět ve formátu bmp, který bude vypsán na standardní výstup, nebo do souboru

### Porovnávání bitových souborů
- možnosti výpisu na standardní výstup:
    - výpis v dekadické formě
    - výpis v hexadecimální formě
- výstupem bude výpis binárního souboru na standardní výstup, nebo do souboru
- výstupem bude odečtení druhého souboru od prvního po bajtech

### Porovnávání textových souborů
- možnost vynutit bitové porovnávání
- porovnávání bude probíhat v rámci řádků
- výstupem bude výpis na standardní výstup, nebo do souboru vyznačující rozdílné řádky

### Porovnávání složek
- pracovní módy:
    - možnost porovnávat pouze strukturu složek
    - možnost porovnávat strukturu složek i s jejich soubory
    - možnost nastavit hloubku prohledávání pro porovnávání
    - možnost vypnutí porovnávání skrytých souborů a složek (soubory a složky začínající tečkou) ve složkách 
- výstupem bude výpis na standardní výstup (,nebo do textového souboru) vyobrazující rozdíly ve struktuře složek popřípadě rozdíly v jejich souborech 

Rozhraní volání programu je popsáno v souboru flags.txt .
Využití polymorfismu bude v ruzných druzích objektů zděděných z objektu DiffObj. Toto bude například využito v porovnávání dvou DiffObj obejktů, v ukládání DiffObj objektů do vnitřní struktury objektu FolderObj, načítání a vypisovnání DiffObj objektu.
