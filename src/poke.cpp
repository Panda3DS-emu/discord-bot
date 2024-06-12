#include "poke.hpp"
#include "colors.h"
#include "dispatcher.h"
#include "message.h"
#include "toml.hpp"
#include <filesystem>
#include <mutex>
#include <unordered_map>

int wishesGambled = 0;
int totalWinnings = 0;
int totalLosses = 0;

namespace poke {

    std::vector<std::string> names = {
        "BIG DUMMY",
        "bulbasaur", 
        "ivysaur", 
        "venusaur", 
        "charmander", 
        "charmeleon", 
        "charizard", 
        "squirtle", 
        "wartortle", 
        "blastoise", 
        "caterpie", 
        "metapod", 
        "butterfree", 
        "weedle", 
        "kakuna", 
        "beedrill", 
        "pidgey", 
        "pidgeotto", 
        "pidgeot", 
        "rattata", 
        "raticate", 
        "spearow", 
        "fearow", 
        "ekans", 
        "arbok", 
        "pikachu", 
        "raichu", 
        "sandshrew", 
        "sandslash", 
        "nidoran-f", 
        "nidorina", 
        "nidoqueen", 
        "nidoran-m", 
        "nidorino", 
        "nidoking", 
        "clefairy", 
        "clefable", 
        "vulpix", 
        "ninetales", 
        "jigglypuff", 
        "wigglytuff", 
        "zubat", 
        "golbat", 
        "oddish", 
        "gloom", 
        "vileplume", 
        "paras", 
        "parasect", 
        "venonat", 
        "venomoth", 
        "diglett", 
        "dugtrio", 
        "meowth", 
        "persian", 
        "psyduck", 
        "golduck", 
        "mankey", 
        "primeape", 
        "growlithe", 
        "arcanine", 
        "poliwag", 
        "poliwhirl", 
        "poliwrath", 
        "abra", 
        "kadabra", 
        "alakazam", 
        "machop", 
        "machoke", 
        "machamp", 
        "bellsprout", 
        "weepinbell", 
        "victreebel", 
        "tentacool", 
        "tentacruel", 
        "geodude", 
        "graveler", 
        "golem", 
        "ponyta", 
        "rapidash", 
        "slowpoke", 
        "slowbro", 
        "magnemite", 
        "magneton", 
        "farfetchd", 
        "doduo", 
        "dodrio", 
        "seel", 
        "dewgong", 
        "grimer", 
        "muk", 
        "shellder", 
        "cloyster", 
        "gastly", 
        "haunter", 
        "gengar", 
        "onix", 
        "drowzee", 
        "hypno", 
        "krabby", 
        "kingler", 
        "voltorb", 
        "electrode", 
        "exeggcute", 
        "exeggutor", 
        "cubone", 
        "marowak", 
        "hitmonlee", 
        "hitmonchan", 
        "lickitung", 
        "koffing", 
        "weezing", 
        "rhyhorn", 
        "rhydon", 
        "chansey", 
        "tangela", 
        "kangaskhan", 
        "horsea", 
        "seadra", 
        "goldeen", 
        "seaking", 
        "staryu", 
        "starmie", 
        "mr-mime", 
        "scyther", 
        "jynx", 
        "electabuzz", 
        "magmar", 
        "pinsir", 
        "tauros", 
        "magikarp", 
        "gyarados", 
        "lapras", 
        "ditto", 
        "eevee", 
        "vaporeon", 
        "jolteon", 
        "flareon", 
        "porygon", 
        "omanyte", 
        "omastar", 
        "kabuto", 
        "kabutops", 
        "aerodactyl", 
        "snorlax", 
        "articuno", 
        "zapdos", 
        "moltres", 
        "dratini", 
        "dragonair", 
        "dragonite", 
        "mewtwo", 
        "mew", 
        "chikorita", 
        "bayleef", 
        "meganium", 
        "cyndaquil", 
        "quilava", 
        "typhlosion", 
        "totodile", 
        "croconaw", 
        "feraligatr", 
        "sentret", 
        "furret", 
        "hoothoot", 
        "noctowl", 
        "ledyba", 
        "ledian", 
        "spinarak", 
        "ariados", 
        "crobat", 
        "chinchou", 
        "lanturn", 
        "pichu", 
        "cleffa", 
        "igglybuff", 
        "togepi", 
        "togetic", 
        "natu", 
        "xatu", 
        "mareep", 
        "flaaffy", 
        "ampharos", 
        "bellossom", 
        "marill", 
        "azumarill", 
        "sudowoodo", 
        "politoed", 
        "hoppip", 
        "skiploom", 
        "jumpluff", 
        "aipom", 
        "sunkern", 
        "sunflora", 
        "yanma", 
        "wooper", 
        "quagsire", 
        "espeon", 
        "umbreon", 
        "murkrow", 
        "slowking", 
        "misdreavus", 
        "unown", 
        "wobbuffet", 
        "girafarig", 
        "pineco", 
        "forretress", 
        "dunsparce", 
        "gligar", 
        "steelix", 
        "snubbull", 
        "granbull", 
        "qwilfish", 
        "scizor", 
        "shuckle", 
        "heracross", 
        "sneasel", 
        "teddiursa", 
        "ursaring", 
        "slugma", 
        "magcargo", 
        "swinub", 
        "piloswine", 
        "corsola", 
        "remoraid", 
        "octillery", 
        "delibird", 
        "mantine", 
        "skarmory", 
        "houndour", 
        "houndoom", 
        "kingdra", 
        "phanpy", 
        "donphan", 
        "porygon2", 
        "stantler", 
        "smeargle", 
        "tyrogue", 
        "hitmontop", 
        "smoochum", 
        "elekid", 
        "magby", 
        "miltank", 
        "blissey", 
        "raikou", 
        "entei", 
        "suicune", 
        "larvitar", 
        "pupitar", 
        "tyranitar", 
        "lugia", 
        "ho-oh", 
        "celebi", 
        "treecko", 
        "grovyle", 
        "sceptile", 
        "torchic", 
        "combusken", 
        "blaziken", 
        "mudkip", 
        "marshtomp", 
        "swampert", 
        "poochyena", 
        "mightyena", 
        "zigzagoon", 
        "linoone", 
        "wurmple", 
        "silcoon", 
        "beautifly", 
        "cascoon", 
        "dustox", 
        "lotad", 
        "lombre", 
        "ludicolo", 
        "seedot", 
        "nuzleaf", 
        "shiftry", 
        "taillow", 
        "swellow", 
        "wingull", 
        "pelipper", 
        "ralts", 
        "kirlia", 
        "gardevoir", 
        "surskit", 
        "masquerain", 
        "shroomish", 
        "breloom", 
        "slakoth", 
        "vigoroth", 
        "slaking", 
        "nincada", 
        "ninjask", 
        "shedinja", 
        "whismur", 
        "loudred", 
        "exploud", 
        "makuhita", 
        "hariyama", 
        "azurill", 
        "nosepass", 
        "skitty", 
        "delcatty", 
        "sableye", 
        "mawile", 
        "aron", 
        "lairon", 
        "aggron", 
        "meditite", 
        "medicham", 
        "electrike", 
        "manectric", 
        "plusle", 
        "minun", 
        "volbeat", 
        "illumise", 
        "roselia", 
        "gulpin", 
        "swalot", 
        "carvanha", 
        "sharpedo", 
        "wailmer", 
        "wailord", 
        "numel", 
        "camerupt", 
        "torkoal", 
        "spoink", 
        "grumpig", 
        "spinda", 
        "trapinch", 
        "vibrava", 
        "flygon", 
        "cacnea", 
        "cacturne", 
        "swablu", 
        "altaria", 
        "zangoose", 
        "seviper", 
        "lunatone", 
        "solrock", 
        "barboach", 
        "whiscash", 
        "corphish", 
        "crawdaunt", 
        "baltoy", 
        "claydol", 
        "lileep", 
        "cradily", 
        "anorith", 
        "armaldo", 
        "feebas", 
        "milotic", 
        "castform", 
        "kecleon", 
        "shuppet", 
        "banette", 
        "duskull", 
        "dusclops", 
        "tropius", 
        "chimecho", 
        "absol", 
        "wynaut", 
        "snorunt", 
        "glalie", 
        "spheal", 
        "sealeo", 
        "walrein", 
        "clamperl", 
        "huntail", 
        "gorebyss", 
        "relicanth", 
        "luvdisc", 
        "bagon", 
        "shelgon", 
        "salamence", 
        "beldum", 
        "metang", 
        "metagross", 
        "regirock", 
        "regice", 
        "registeel", 
        "latias", 
        "latios", 
        "kyogre", 
        "groudon", 
        "rayquaza", 
        "jirachi", 
        "deoxys-normal", 
        "turtwig", 
        "grotle", 
        "torterra", 
        "chimchar", 
        "monferno", 
        "infernape", 
        "piplup", 
        "prinplup", 
        "empoleon", 
        "starly", 
        "staravia", 
        "staraptor", 
        "bidoof", 
        "bibarel", 
        "kricketot", 
        "kricketune", 
        "shinx", 
        "luxio", 
        "luxray", 
        "budew", 
        "roserade", 
        "cranidos", 
        "rampardos", 
        "shieldon", 
        "bastiodon", 
        "burmy", 
        "wormadam-plant", 
        "mothim", 
        "combee", 
        "vespiquen", 
        "pachirisu", 
        "buizel", 
        "floatzel", 
        "cherubi", 
        "cherrim", 
        "shellos", 
        "gastrodon", 
        "ambipom", 
        "drifloon", 
        "drifblim", 
        "buneary", 
        "lopunny", 
        "mismagius", 
        "honchkrow", 
        "glameow", 
        "purugly", 
        "chingling", 
        "stunky", 
        "skuntank", 
        "bronzor", 
        "bronzong", 
        "bonsly", 
        "mime-jr", 
        "happiny", 
        "chatot", 
        "spiritomb", 
        "gible", 
        "gabite", 
        "garchomp", 
        "munchlax", 
        "riolu", 
        "lucario", 
        "hippopotas", 
        "hippowdon", 
        "skorupi", 
        "drapion", 
        "croagunk", 
        "toxicroak", 
        "carnivine", 
        "finneon", 
        "lumineon", 
        "mantyke", 
        "snover", 
        "abomasnow", 
        "weavile", 
        "magnezone", 
        "lickilicky", 
        "rhyperior", 
        "tangrowth", 
        "electivire", 
        "magmortar", 
        "togekiss", 
        "yanmega", 
        "leafeon", 
        "glaceon", 
        "gliscor", 
        "mamoswine", 
        "porygon-z", 
        "gallade", 
        "probopass", 
        "dusknoir", 
        "froslass", 
        "rotom", 
        "uxie", 
        "mesprit", 
        "azelf", 
        "dialga", 
        "palkia", 
        "heatran", 
        "regigigas", 
        "giratina-altered", 
        "cresselia", 
        "phione", 
        "manaphy", 
        "darkrai", 
        "shaymin-land", 
        "arceus", 
        "victini", 
        "snivy", 
        "servine", 
        "serperior", 
        "tepig", 
        "pignite", 
        "emboar", 
        "oshawott", 
        "dewott", 
        "samurott", 
        "patrat", 
        "watchog", 
        "lillipup", 
        "herdier", 
        "stoutland", 
        "purrloin", 
        "liepard", 
        "pansage", 
        "simisage", 
        "pansear", 
        "simisear", 
        "panpour", 
        "simipour", 
        "munna", 
        "musharna", 
        "pidove", 
        "tranquill", 
        "unfezant", 
        "blitzle", 
        "zebstrika", 
        "roggenrola", 
        "boldore", 
        "gigalith", 
        "woobat", 
        "swoobat", 
        "drilbur", 
        "excadrill", 
        "audino", 
        "timburr", 
        "gurdurr", 
        "conkeldurr", 
        "tympole", 
        "palpitoad", 
        "seismitoad", 
        "throh", 
        "sawk", 
        "sewaddle", 
        "swadloon", 
        "leavanny", 
        "venipede", 
        "whirlipede", 
        "scolipede", 
        "cottonee", 
        "whimsicott", 
        "petilil", 
        "lilligant", 
        "basculin-red-striped", 
        "sandile", 
        "krokorok", 
        "krookodile", 
        "darumaka", 
        "darmanitan-standard", 
        "maractus", 
        "dwebble", 
        "crustle", 
        "scraggy", 
        "scrafty", 
        "sigilyph", 
        "yamask", 
        "cofagrigus", 
        "tirtouga", 
        "carracosta", 
        "archen", 
        "archeops", 
        "trubbish", 
        "garbodor", 
        "zorua", 
        "zoroark", 
        "minccino", 
        "cinccino", 
        "gothita", 
        "gothorita", 
        "gothitelle", 
        "solosis", 
        "duosion", 
        "reuniclus", 
        "ducklett", 
        "swanna", 
        "vanillite", 
        "vanillish", 
        "vanilluxe", 
        "deerling", 
        "sawsbuck", 
        "emolga", 
        "karrablast", 
        "escavalier", 
        "foongus", 
        "amoonguss", 
        "frillish", 
        "jellicent", 
        "alomomola", 
        "joltik", 
        "galvantula", 
        "ferroseed", 
        "ferrothorn", 
        "klink", 
        "klang", 
        "klinklang", 
        "tynamo", 
        "eelektrik", 
        "eelektross", 
        "elgyem", 
        "beheeyem", 
        "litwick", 
        "lampent", 
        "chandelure", 
        "axew", 
        "fraxure", 
        "haxorus", 
        "cubchoo", 
        "beartic", 
        "cryogonal", 
        "shelmet", 
        "accelgor", 
        "stunfisk", 
        "mienfoo", 
        "mienshao", 
        "druddigon", 
        "golett", 
        "golurk", 
        "pawniard", 
        "bisharp", 
        "bouffalant", 
        "rufflet", 
        "braviary", 
        "vullaby", 
        "mandibuzz", 
        "heatmor", 
        "durant", 
        "deino", 
        "zweilous", 
        "hydreigon", 
        "larvesta", 
        "volcarona", 
        "cobalion", 
        "terrakion", 
        "virizion", 
        "tornadus-incarnate", 
        "thundurus-incarnate", 
        "reshiram", 
        "zekrom", 
        "landorus-incarnate", 
        "kyurem", 
        "keldeo-ordinary", 
        "meloetta-aria", 
        "genesect", 
        "chespin", 
        "quilladin", 
        "chesnaught", 
        "fennekin", 
        "braixen", 
        "delphox", 
        "froakie", 
        "frogadier", 
        "greninja", 
        "bunnelby", 
        "diggersby", 
        "fletchling", 
        "fletchinder", 
        "talonflame", 
        "scatterbug", 
        "spewpa", 
        "vivillon", 
        "litleo", 
        "pyroar", 
        "flabebe", 
        "floette", 
        "florges", 
        "skiddo", 
        "gogoat", 
        "pancham", 
        "pangoro", 
        "furfrou", 
        "espurr", 
        "meowstic-male", 
        "honedge", 
        "doublade", 
        "aegislash-shield", 
        "spritzee", 
        "aromatisse", 
        "swirlix", 
        "slurpuff", 
        "inkay", 
        "malamar", 
        "binacle", 
        "barbaracle", 
        "skrelp", 
        "dragalge", 
        "clauncher", 
        "clawitzer", 
        "helioptile", 
        "heliolisk", 
        "tyrunt", 
        "tyrantrum", 
        "amaura", 
        "aurorus", 
        "sylveon", 
        "hawlucha", 
        "dedenne", 
        "carbink", 
        "goomy", 
        "sliggoo", 
        "goodra", 
        "klefki", 
        "phantump", 
        "trevenant", 
        "pumpkaboo-average", 
        "gourgeist-average", 
        "bergmite", 
        "avalugg", 
        "noibat", 
        "noivern", 
        "xerneas", 
        "yveltal", 
        "zygarde-50", 
        "diancie", 
        "hoopa", 
        "volcanion", 
        "rowlet", 
        "dartrix", 
        "decidueye", 
        "litten", 
        "torracat", 
        "incineroar", 
        "popplio", 
        "brionne", 
        "primarina", 
        "pikipek", 
        "trumbeak", 
        "toucannon", 
        "yungoos", 
        "gumshoos", 
        "grubbin", 
        "charjabug", 
        "vikavolt", 
        "crabrawler", 
        "crabominable", 
        "oricorio-baile", 
        "cutiefly", 
        "ribombee", 
        "rockruff", 
        "lycanroc-midday", 
        "wishiwashi-solo", 
        "mareanie", 
        "toxapex", 
        "mudbray", 
        "mudsdale", 
        "dewpider", 
        "araquanid", 
        "fomantis", 
        "lurantis", 
        "morelull", 
        "shiinotic", 
        "salandit", 
        "salazzle", 
        "stufful", 
        "bewear", 
        "bounsweet", 
        "steenee", 
        "tsareena", 
        "comfey", 
        "oranguru", 
        "passimian", 
        "wimpod", 
        "golisopod", 
        "sandygast", 
        "palossand", 
        "pyukumuku", 
        "type-null", 
        "silvally", 
        "minior-red-meteor", 
        "komala", 
        "turtonator", 
        "togedemaru", 
        "mimikyu-disguised", 
        "bruxish", 
        "drampa", 
        "dhelmise", 
        "jangmo-o", 
        "hakamo-o", 
        "kommo-o", 
        "tapu-koko", 
        "tapu-lele", 
        "tapu-bulu", 
        "tapu-fini", 
        "cosmog", 
        "cosmoem", 
        "solgaleo", 
        "lunala", 
        "nihilego", 
        "buzzwole", 
        "pheromosa", 
        "xurkitree", 
        "celesteela", 
        "kartana", 
        "guzzlord", 
        "necrozma", 
        "magearna", 
        "marshadow", 
        "poipole", 
        "naganadel", 
        "stakataka", 
        "blacephalon", 
        "zeraora", 
        "meltan", 
        "melmetal", 
        "grookey", 
        "thwackey", 
        "rillaboom", 
        "scorbunny", 
        "raboot", 
        "cinderace", 
        "sobble", 
        "drizzile", 
        "inteleon", 
        "skwovet", 
        "greedent", 
        "rookidee", 
        "corvisquire", 
        "corviknight", 
        "blipbug", 
        "dottler", 
        "orbeetle", 
        "nickit", 
        "thievul", 
        "gossifleur", 
        "eldegoss", 
        "wooloo", 
        "dubwool", 
        "chewtle", 
        "drednaw", 
        "yamper", 
        "boltund", 
        "rolycoly", 
        "carkol", 
        "coalossal", 
        "applin", 
        "flapple", 
        "appletun", 
        "silicobra", 
        "sandaconda", 
        "cramorant", 
        "arrokuda", 
        "barraskewda", 
        "toxel", 
        "toxtricity-amped", 
        "sizzlipede", 
        "centiskorch", 
        "clobbopus", 
        "grapploct", 
        "sinistea", 
        "polteageist", 
        "hatenna", 
        "hattrem", 
        "hatterene", 
        "impidimp", 
        "morgrem", 
        "grimmsnarl", 
        "obstagoon", 
        "perrserker", 
        "cursola", 
        "sirfetchd", 
        "mr-rime", 
        "runerigus", 
        "milcery", 
        "alcremie", 
        "falinks", 
        "pincurchin", 
        "snom", 
        "frosmoth", 
        "stonjourner", 
        "eiscue-ice", 
        "indeedee-male", 
        "morpeko-full-belly", 
        "cufant", 
        "copperajah", 
        "dracozolt", 
        "arctozolt", 
        "dracovish", 
        "arctovish", 
        "duraludon", 
        "dreepy", 
        "drakloak", 
        "dragapult", 
        "zacian", 
        "zamazenta", 
        "eternatus", 
        "kubfu", 
        "urshifu-single-strike", 
        "zarude", 
        "regieleki", 
        "regidrago", 
        "glastrier", 
        "spectrier", 
        "calyrex", 
        "wyrdeer", 
        "kleavor", 
        "ursaluna", 
        "basculegion-male", 
        "sneasler", 
        "overqwil", 
        "enamorus-incarnate", 
        "sprigatito", 
        "floragato", 
        "meowscarada", 
        "fuecoco", 
        "crocalor", 
        "skeledirge", 
        "quaxly", 
        "quaxwell", 
        "quaquaval", 
        "lechonk", 
        "oinkologne", 
        "tarountula", 
        "spidops", 
        "nymble", 
        "lokix", 
    };

    std::vector<int> legendaries = {
        144, 145, 146, 150, 151, 243, 244, 245, 249, 250, 251, 
        377, 378, 379, 380, 381, 382, 383, 384, 385, 386, 
        480, 481, 482, 483, 484, 485, 486, 487, 488, 489, 
        490, 491, 492, 493, 638, 639, 640, 641, 642, 643, 
        644, 645, 646, 647, 648, 649, 716, 717, 718, 719, 
        720, 721, 785, 786, 787, 788, 789, 790, 791, 792, 
        800, 801, 802, 807, 888, 889, 890, 891, 892, 894, 
        895, 896, 897, 898,

        149,  // Dragonite
        248,  // Tyranitar
        373,  // Salamence
        376,  // Metagross
        445,  // Garchomp
        635,  // Hydreigon
        706,  // Goodra
        784,  // Kommo-o
        883,  // Dragapult
        997,  // Baxcalibur

        3,    // Venusaur
        6,    // Charizard
        9,    // Blastoise
        65,   // Alakazam
        94,   // Gengar
        95,   // Onix
        112,  // Rhydon
        130,  // Gyarados
        131,  // Lapras
        143,  // Snorlax
        197,  // Umbreon
        208,  // Steelix
        212,  // Scizor
        282,  // Gardevoir
        330,  // Flygon
        448,  // Lucario
        466,  // Electivire
        467,  // Magmortar
        477,  // Dusknoir
        530,  // Excadrill
        571,  // Zoroark
        609,  // Chandelure
        745,  // Lycanroc
        763,  // Tsareena
        859,  // Grimmsnarl

        25,   // Pikachu
        172,  // Pichu
        201,  // Unown
        282,  // Gardevoir
        134,  // Vaporeon
        135,  // Jolteon
        136,  // Flareon
        196,  // Espeon
        197,  // Umbreon
        470,  // Leafeon
        471,  // Glaceon
        700,  // Sylveon
    };
    
    time_t lastBanner;
    int bannerPokemon;

    bool leaderboardInvalidated = true;
    std::vector<std::pair<uint64_t, int>> leaderboard;

    using Pokemon = std::vector<std::string>;

    enum PokemonIndex {
        ID = 0,
        SHINY = 1,

        SIZE,
    };

    struct User {
        int wishes;
        std::vector<Pokemon> pokemon;
        time_t daily;
        time_t autoclaim;
        int pity;
    };

    std::unordered_map<uint64_t, User> users;

    std::mutex mtx;

    std::string getPath(uint64_t id)
    {
        return "poke/" + std::to_string(id) + ".toml";
    }

    void Initialize() {
        srand(time(nullptr));
        std::filesystem::create_directory("poke");
        auto dirIterator = std::filesystem::directory_iterator("poke");

        for (const auto& entry : dirIterator) {
            auto table = toml::parse(entry.path().string());

            User user;
            user.wishes = toml::get<int>(table["wishes"]);
            user.daily = toml::get<time_t>(table["daily"]);
            user.pokemon = toml::get<std::vector<Pokemon>>(table["pokemon"]);
            user.pity = toml::get<int>(table["pity"]);

            for (auto& pokemon : user.pokemon)
            {
                if (pokemon.size() < PokemonIndex::SIZE)
                {
                    pokemon.resize(PokemonIndex::SIZE);
                }

                if (pokemon[SHINY] != "true" && pokemon[SHINY] != "false")
                {
                    pokemon[SHINY] = "false";
                }
            }

            users[std::stoull(entry.path().filename().string())] = user;
        }

        if (std::filesystem::exists("banner.toml")) {
            auto table = toml::parse("banner.toml");
            lastBanner = toml::get<time_t>(table["lastBanner"]);
            bannerPokemon = toml::get<int>(table["bannerPokemon"]);
        } else {
            lastBanner = time(nullptr);
            bannerPokemon = legendaries[rand() % legendaries.size()];

            toml::table table;
            table["lastBanner"] = lastBanner;
            table["bannerPokemon"] = bannerPokemon;

            std::ofstream file("banner.toml");
            file << toml::value(table);
        }

        if (std::filesystem::exists("stats.toml")) {
            auto table = toml::parse("stats.toml");
            wishesGambled = toml::get<int>(table["wishesGambled"]);
            totalWinnings = toml::get<int>(table["totalWinnings"]);
            totalLosses = toml::get<int>(table["totalLosses"]);
        } else {
            toml::table table;
            table["wishesGambled"] = wishesGambled;
            table["totalWinnings"] = totalWinnings;
            table["totalLosses"] = totalLosses;

            std::ofstream file("stats.toml");
            file << toml::value(table);
        }
    }

    void CheckAndCreateUser(uint64_t nid)
    {
        if (time(nullptr) - lastBanner > 259200)
        {
            lastBanner = time(nullptr);
            bannerPokemon = legendaries[rand() % legendaries.size()];

            toml::table table;
            table["lastBanner"] = lastBanner;
            table["bannerPokemon"] = bannerPokemon;

            std::ofstream file("banner.toml");
            file << toml::value(table);
        }

        std::string id = std::to_string(nid);

        if (users.find(std::stoull(id)) != users.end())
        {
            time_t now = time(nullptr);
            time_t daily = users[std::stoull(id)].daily;

            time_t diff = now - daily;
            time_t interval = 86400 / 4;

            if (diff >= interval)
            {
                int newWishes = (diff / interval) * 5;
                if (newWishes > 30)
                {
                    newWishes = 30;
                }

                users[std::stoull(id)].wishes += newWishes;
                users[std::stoull(id)].daily = time(nullptr) + (diff % interval);
            }

            toml::table table;
            table["wishes"] = users[std::stoull(id)].wishes;
            table["daily"] = users[std::stoull(id)].daily;
            table["pokemon"] = users[std::stoull(id)].pokemon;
            table["pity"] = users[std::stoull(id)].pity;

            std::ofstream file(getPath(nid));
            file << toml::value(table);
            return;
        } else {
            User user;
            user.wishes = 10;
            user.daily = time(nullptr);
            user.pokemon = {};
            user.pity = 0;
            users[std::stoull(id)] = user;

            toml::table table;
            table["wishes"] = user.wishes;
            table["daily"] = user.daily;
            table["pokemon"] = user.pokemon;
            table["pity"] = user.pity;

            std::ofstream file(getPath(nid));
            file << toml::value(table);
        }
    }

    void Daily(const dpp::slashcommand_t& event)
    {
        event.reply("/daily has been replaced with an automatic claiming system. Every 6 hours five wishes are added to your account, up to 30 wishes since your last interaction with the bot. You can check your wishes with /wishes.");
        // std::lock_guard<std::mutex> lock(mtx);
        // uint64_t id = event.command.get_issuing_user().id;
        // CheckAndCreateUser(id);

        // time_t now = time(nullptr);
        // time_t daily = users[id].daily;

        // if (now - daily < 86400 / 4)
        // {
        //     event.reply("You already claimed your daily reward. Next claim <t:" + std::to_string(daily + 86400 / 4) + ":R>.");
        // } else {
        //     users[id].daily = now;
        //     users[id].wishes += 5;

        //     std::ofstream file(getPath(id));
        //     toml::table table;
        //     table["wishes"] = users[id].wishes;
        //     table["daily"] = users[id].daily;
        //     table["pokemon"] = users[id].pokemon;
        //     table["pity"] = users[id].pity;
        //     file << toml::value(table);

        //     event.reply("You claimed your daily reward. You now have " + std::to_string(users[id].wishes) + " wishes. Next daily reset <t:" + std::to_string(now + 86400 / 4) + ":R>.");
        // }
    }

    void Wishes(const dpp::slashcommand_t& event)
    {
        std::lock_guard<std::mutex> lock(mtx);
        uint64_t id = event.command.get_issuing_user().id;
        CheckAndCreateUser(id);

        int wishes = users[id].wishes;
        time_t timeUntil;
        auto diff = time(nullptr) - users[id].daily;
        timeUntil = diff % (86400 / 4);
        event.reply("You have " + std::to_string(wishes) + " wishes. You get 5 more wishes <t:" + std::to_string(time(nullptr) + diff) + ":R>.");
    }

    void Wish(const dpp::slashcommand_t& event)
    {
        std::lock_guard<std::mutex> lock(mtx);
        leaderboardInvalidated = true;
        uint64_t id = event.command.get_issuing_user().id;
        CheckAndCreateUser(id);

        int wishes = users[id].wishes;
        
        if (wishes == 0)
        {
            time_t timeUntil;
            auto diff = time(nullptr) - users[id].daily;
            timeUntil = diff % (86400 / 4);
            event.reply("You have no wishes left. You get 5 more wishes <t:" + std::to_string(time(nullptr) + diff) + ":R>.");
        } else {
            int roll1 = (rand() % 920) + 1;
            int roll2 = (rand() % 920) + 1;
            bool shiny = (rand() % 128) == 0;
            bool lucky = (rand() % 40) == 0;
            bool luckier = (rand() % 256) == 0;
            bool luckiest = (rand() % 1024) == 0;
            bool legendary = false;
            int roll;

            if (roll1 == bannerPokemon || roll2 == bannerPokemon)
            {
                roll = bannerPokemon;
                legendary = true;
            } else {
                roll = roll1;

                // Reroll once if legendary, to make them rarer
                if (std::find(legendaries.begin(), legendaries.end(), roll) != legendaries.end())
                {
                    roll = (rand() % 920) + 1;
                }

                if (std::find(legendaries.begin(), legendaries.end(), roll) != legendaries.end())
                {
                    legendary = true;
                }
            }

            if (users[id].pity >= 50)
            {
                users[id].pity = 0;
                roll = bannerPokemon;
                legendary = true;
            }
            
            std::string url = "https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/pokemon/other/showdown/" + std::string(shiny ? "shiny/" : "") + std::to_string(roll) + ".gif";
            dpp::embed embed = dpp::embed()
                .set_image(url);

            users[id].wishes--;

            std::string shinyText = shiny ? "SHINY! ✨ " : "";

            std::string name = "FATAL ERROR";
            if (roll < names.size()) {
                name = names[roll];
            }

            name[0] = toupper(name[0]);

            bool rude = (rand() % 25) == 0;
            if (rude) {
                int r = rand() % 3;
                switch (r) {
                    case 0: name = "fucking " + name; break;
                    case 1: name = "god damned " + name; break;
                    case 2: name = "useless " + name; break;
                }
            }

            if (legendary)
            {
                embed.set_title(shinyText + "WOAH! You got a " + name + "! ");
                embed.set_color(dpp::colors::orange);
            } else {
                embed.set_title(shinyText + "You got a " + name + ".");
                embed.set_color(dpp::colors::green);
            }

            bool found = false;

            for (auto& pokemon : users[id].pokemon)
            {
                if (pokemon[ID] == std::to_string(roll))
                {
                    found = true;
                    break;
                }
            }

            std::string footer;

            if (!found)
            {
                Pokemon pokemon;
                pokemon.push_back(std::to_string(roll));
                pokemon.push_back(shiny ? "true" : "false");
                users[id].pokemon.push_back(pokemon);
            } else {
                if (shiny) {
                    footer += "You already have this pokemon as a non-shiny, so your non-shiny variant decomposed into 1 wish.\n";
                    users[id].wishes++;
                    
                    for (auto& pokemon : users[id].pokemon)
                    {
                        if (pokemon[ID] == std::to_string(roll))
                        {
                            pokemon[SHINY] = "true";
                            break;
                        }
                    }
                } else {
                    bool decompose = (rand() % 3) == 0;

                    if (decompose)
                    {
                        footer += "You already have this pokemon so it decomposed into 1 wish.\n";
                        users[id].wishes++;
                    } else {
                        footer += "You already have this pokemon so it decomposed.\n";
                    }
                }
            }

            if (lucky)
            {
                footer += "Today is your lucky day! At a 2.5% chance you got 5 free wishes!\n";
                users[id].wishes += 5;
            }

            if (luckier)
            {
                footer += "Today is your lucky day! At a 0.4% chance you got 20 free wishes!\n";
                users[id].wishes += 20;
            }

            if (luckiest)
            {
                footer += "HOLY MOLY! Today is your luckiest day! At a 0.1% chance you got 50 free wishes!\n";
                users[id].wishes += 50;
            }

            embed.set_footer(footer, "");

            if (roll == bannerPokemon)
            {
                users[id].pity = 0;
            } else {
                users[id].pity++;
            }

            event.reply(dpp::message(event.command.channel_id, embed));

            std::ofstream file(getPath(id));
            toml::table table;
            table["wishes"] = users[id].wishes;
            table["daily"] = users[id].daily;
            table["pokemon"] = users[id].pokemon;
            table["pity"] = users[id].pity;
            file << toml::value(table);
        }
    }

    void List(const dpp::slashcommand_t& event)
    {
        std::lock_guard<std::mutex> lock(mtx);
        uint64_t id = event.command.get_issuing_user().id;
        CheckAndCreateUser(id);

        dpp::embed embed = dpp::embed()
            .set_title("You have " + std::to_string(users[id].pokemon.size()) + " Pokemon.")
            .set_color(0x00FF00);

        if (users[id].pokemon.size() > 0) {
            bool shiny = users[id].pokemon[0][SHINY] == "true";
            embed.set_thumbnail("https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/pokemon/other/showdown/" + std::string(shiny ? "shiny/" : "") + users[id].pokemon[0][ID] + ".gif");
        }

        std::string description = "";
        for (auto& pokemon : users[id].pokemon)
        {
            std::string name = "FATAL ERROR";
            if (std::stoi(pokemon[ID]) < names.size()) {
                name = names[std::stoi(pokemon[ID])];
            }
            name[0] = toupper(name[0]);
            description += "#" + pokemon[ID] + " - " + name + " " + (pokemon[SHINY] == "true" ? "✨" : "") + "\n";
        }

        embed.set_description(description);

        event.reply(dpp::message(event.command.channel_id, embed));
    }

    void Favorite(const dpp::slashcommand_t& event)
    {
        std::lock_guard<std::mutex> lock(mtx);
        uint64_t id = event.command.get_issuing_user().id;
        CheckAndCreateUser(id);

        std::string pokemon = std::get<std::string>(event.get_parameter("pokemon"));

        bool found = false;

        Pokemon selectedPokemon;

        for (auto& p : users[id].pokemon)
        {
            if (p[ID] == pokemon)
            {
                found = true;
                selectedPokemon = p;
                break;
            }
        }

        if (!found)
        {
            event.reply("You don't have that Pokemon.");
        } else {
            users[id].pokemon.erase(std::remove(users[id].pokemon.begin(), users[id].pokemon.end(), selectedPokemon), users[id].pokemon.end());
            users[id].pokemon.insert(users[id].pokemon.begin(), selectedPokemon);

            std::ofstream file(getPath(id));
            toml::table table;
            table["wishes"] = users[id].wishes;
            table["daily"] = users[id].daily;
            table["pokemon"] = users[id].pokemon;
            table["pity"] = users[id].pity;
            file << toml::value(table);

            event.reply("You favorited " + pokemon + ".");
        }
    }

    void Banner(const dpp::slashcommand_t& event)
    {
        std::lock_guard<std::mutex> lock(mtx);
        uint64_t id = event.command.get_issuing_user().id;
        CheckAndCreateUser(id);

        std::string url = "https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/pokemon/other/showdown/" + std::to_string(bannerPokemon) + ".gif";

        dpp::embed embed = dpp::embed()
            .set_thumbnail(url)
            .set_title("Today's banner is #" + std::to_string(bannerPokemon) + "!")
            .set_description("This pokemon has double chance to appear when you wish and you are guaranteed to get it after 50 wishes.\n\nNext banner reset: <t:" + std::to_string(lastBanner + 259200) + ":R>.")
            .set_footer("Wishes left until banner pull: " + std::to_string(50 - users[id].pity), "")
            .set_color(dpp::colors::orange);
        
        event.reply(dpp::message(event.command.channel_id, embed));
    }

    void Leaderboard(const dpp::slashcommand_t &event)
    {
        std::lock_guard<std::mutex> lock(mtx);
        uint64_t id = event.command.get_issuing_user().id;
        CheckAndCreateUser(id);

        if (leaderboardInvalidated) {
            leaderboard.clear();

            for (auto& user : users)
            {
                int sum = 0;
                for (auto& pokemon : user.second.pokemon)
                {
                    if (pokemon[SHINY] == "true")
                    {
                        sum += 5;
                    } else {
                        sum++;
                    }

                    if (std::find(legendaries.begin(), legendaries.end(), std::stoi(pokemon[ID])) != legendaries.end())
                    {
                        sum += 10;
                    }
                }

                leaderboard.push_back({ user.first, sum });
            }

            std::sort(leaderboard.begin(), leaderboard.end(), [](const std::pair<uint64_t, int>& a, const std::pair<uint64_t, int>& b) {
                return a.second > b.second;
            });

            leaderboardInvalidated = false;
        }

        dpp::embed embed = dpp::embed()
            .set_title("Leaderboard")
            .set_color(0x00FF00);

        std::string description = "";
        int i = 1;

        for (auto& user : leaderboard)
        {
            description += std::to_string(i) + ". <@" + std::to_string(user.first) + "> - " + std::to_string(user.second) + " points\n";
            i++;

            if (i > 10)
            {
                break;
            }
        }

        embed.set_description(description);

        event.reply(dpp::message(event.command.channel_id, embed));
    }

    void Roulette(const dpp::slashcommand_t& event)
    {
        std::lock_guard<std::mutex> lock(mtx);
        uint64_t id = event.command.get_issuing_user().id;
        CheckAndCreateUser(id);

        int wishesOnRed = std::get<int64_t>(event.get_parameter("wishes_on_red"));
        int wishesOnBlack = std::get<int64_t>(event.get_parameter("wishes_on_black"));

        if (wishesOnRed < 0 || wishesOnBlack < 0)
        {
            event.reply("You can't bet negative wishes. Your favorite pokemon has been decomposed as a punishment.");
            return;
        }

        if (wishesOnRed + wishesOnBlack == 0) {
            event.reply("You have to bet at least 1 wish. Your favorite pokemon has been decomposed as a punishment.");
            return;
        }

        if (wishesOnRed + wishesOnBlack > users[id].wishes)
        {
            event.reply("You don't have enough wishes. Your favorite pokemon has been decomposed as a punishment.");
            return;
        }

        if (wishesOnRed + wishesOnBlack > 20) {
            event.reply("You can't bet more than 20 wishes. No martingale system for you!");
            return;
        }

        users[id].wishes -= wishesOnRed + wishesOnBlack;

        int roll = rand() % 37;
        bool even = roll % 2 == 0;
        std::string result;

        if (roll >= 1 && roll <= 10) {
            if (even) {
                result = "black";
            } else {
                result = "red";
            }
        } else if (roll >= 11 && roll <= 18) {
            if (even) {
                result = "red";
            } else {
                result = "black";
            }
        } else if (roll >= 19 && roll <= 28) {
            if (even) {
                result = "black";
            } else {
                result = "red";
            }
        } else if (roll != 0) {
            if (even) {
                result = "red";
            } else {
                result = "black";
            }
        }

        if (result.empty()) {
            result = "green";
        }

        int winnings = 0;

        if (result == "red") {
            winnings = wishesOnRed * 2;
        } else if (result == "black") {
            winnings = wishesOnBlack * 2;
        }

        users[id].wishes += winnings;

        std::ofstream file(getPath(id));
        toml::table table;
        table["wishes"] = users[id].wishes;
        table["daily"] = users[id].daily;
        table["pokemon"] = users[id].pokemon;
        table["pity"] = users[id].pity;
        file << toml::value(table);

        dpp::embed embed = dpp::embed()
            .set_title("Roulette 🎰")
            .set_color(0x00FF00);

        if (winnings > 0) {
            embed.set_description("The roulette landed on " + std::to_string(roll) + " (" + result + "). You won " + std::to_string(winnings) + " wishes.");
        } else {
            embed.set_description("The roulette landed on " + std::to_string(roll) + " (" + result + "). You lost " + std::to_string(wishesOnRed + wishesOnBlack) + " wishes.");
        }

        wishesGambled += wishesOnRed + wishesOnBlack;
        if (winnings > wishesOnRed + wishesOnBlack) {
            totalWinnings += winnings - (wishesOnRed + wishesOnBlack);
        } else if (winnings < wishesOnRed + wishesOnBlack) {
            totalLosses += (wishesOnRed + wishesOnBlack) - winnings;
        }

        float fchance = (float)totalWinnings / (float)wishesGambled * 100.0f;
        std::stringstream stream;
        stream << std::fixed << std::setprecision(2) << fchance;
        std::string chance = stream.str();

        embed.set_footer("On this server, " + std::to_string(wishesGambled) + " wishes have been gambled, with a total of " + std::to_string(totalWinnings) + " wishes won and " + std::to_string(totalLosses) + " wishes lost. Expected win rate: ~48.64%. Actual win rate: ~" + chance + "%.", "");

        event.reply(dpp::message(event.command.channel_id, embed));

        std::ofstream statsFile("stats.toml");
        toml::table statsTable;
        statsTable["wishesGambled"] = wishesGambled;
        statsTable["totalWinnings"] = totalWinnings;
        statsTable["totalLosses"] = totalLosses;
        statsFile << toml::value(statsTable);
    }

}