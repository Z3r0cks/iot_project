const char playerQuestion[] = R"=====(<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="stylesheet" href="style.css">
    <title>Frage</title>
</head>

<style>
    body {
        font-size: unset;
    }
</style>

<body>
    <div class="wrapper">
        <div class="headerWrapper questionHeaderWrapper">
            <p class="header questionHeader" id="question">Was macht den Schwarzwald so
                bienenfreundlich?</p>
        </div>
        <div id="answers">
            <div class="answerWrapperBlock">
                <div class="oneAnswerWrapper">
                    <p class="questionTitle margin-right">Antwort A</p>
                    <div class="answerWrapper margin-right">
                        <span id="answerA">Die vielen Höhenmeter, die Bienen gerne haben.</span>
                    </div>
                </div>
                <div class="oneAnswerWrapper">
                    <p class="margin-left questionTitle">Antwort B</p>
                    <div class="answerWrapper margin-left">
                        <span id="answerB">Die gute Luftqualität im Schwarzwald.</span>
                    </div>
                </div>
            </div>
            <div class="answerWrapperBlock">
                <div class="oneAnswerWrapper">
                    <p class="margin-right questionTitle">Antwort C</p>
                    <div class="answerWrapper margin-right">
                        <span id="answerC">Das viele Löwenzahn, das im Schwarzwald wächst.</span>
                    </div>
                </div>
                <div class="oneAnswerWrapper">
                    <p class="margin-left questionTitle">Antwort D</p>
                    <div class="answerWrapper margin-left">
                        <span id="answerD">Der häufige und viele Schneefall im Schwarzwald.</span>
                    </div>
                </div>
            </div>
        </div>
        <button id="login-answer">Antwort einloggen</button>

        <div id="correct-answer">
            <p>A: Baum</p>
            <p id="explenation">Lorem ipsum dolor sit amet consectetur adipisicing elit. Maiores officia molestiae
                corrupti. Minima quia accusamus vero temporibus atque animi deleniti corporis dolores magnam voluptate a
                et, quasi ratione! Natus, distinctio?</p>
        </div>
    </div>
</body>

<!-- <body>
    <h1>Frage beantworten</h1>
    <p id="question">Frage</p>
    <div id="answers">
        <span id="answerA">Test</span>
        <span id="answerB">TEst2</span>
        <span id="answerC">fdsfds</span>
        <span id="answerD">adsad</span>
    </div>
    <button id="login-answer">Antwort einloggen</button>

    <div id="correct-answer">
        <p>A: Baum</p>
        <p id="explenation"></p>
    </div>
</body> -->
<script>
    document.getElementById('answerA').addEventListener('click', function () {
        selectAnswer(this);
    });
    document.getElementById('answerB').addEventListener('click', function () {
        selectAnswer(this);
    });
    document.getElementById('answerC').addEventListener('click', function () {
        selectAnswer(this);
    });
    document.getElementById('answerD').addEventListener('click', function () {
        selectAnswer(this);
    });

    var selectedAnswerGlobal

    var questionID = "$questionID";

    const myArray = [
        "01_01", [
            "Warum ist der Schwarzwald so Bienenfreundlich?",
            "Wegen der Höhenlage",
            "Wegen dem vielen Schnee",
            "Es wächst viel Löwenzahn in der Gegend",
            "Wegen der guten Luft",
            "Löwenzahn besitzen nicht nur eine Blüte sondern viele. Bienen werden davon angelockt.",
            "C"
        ],
        "01_02", [
            "Wieso verlieren Tannen im Winter nicht ihre Nadeln?",
            "Weil Tannen besser Wärme speichern können",
            "Weil die Nadeln eng beieinander liegen und sich dadurch gegenseitig wärmen",
            "Durch eine Art Salz, was als Frostschutzmittel dient",
            "Durch eine Art Zucker, was als Frostschutzmittel dient",
            "Tannen speichen außerdem weniger Wasser und die Nadeln sind dicker. Bei Laubblätter haben kaum festes Gewebe und würden im Winter deshalb erfrieren.",
            "D"
        ],
        "02_01", [
            "Von was ernähren sich Rehe von März bis April?",
            "Bärlappgewächse ",
            "Beeren",
            "Gräser und Knospen",
            "Zweikeimblättrige Kräute",
            "Gräser und Knospen von Mitte März bis Ende April. Laubtriebe und einkeimblättrige Kräuter von Anfang Mai bis Ende Juni. Zweikeimblättrige Kräuter und Laubtriebe von Mitte Juni bis Mitte Oktober. In dieser Zeit ist die Zahl der als Äsungspflanzen generell in Frage kommenden Arten am größten und umfasst rund 134 verschiedene Arten. Schachtelhalme, Farne und Bärlappgewächse sowie Knospen und Brombeeren von Mitte Oktober bis Mitte Dezember. Gräser, Knospen und Brombeeren von Anfang Januar bis Mitte März.",
            "C"
        ],
        "03_01", [
            "Aus was entstand die Hochschule Furtwangen?",
            "Uhrmacherschule", "Baumschule",
            "Katholisches Kloster",
            "Volkshochschulkurs",
            "1850 wurde die Großherzogliche Badische Uhrmacherschule Furtwangen von Robert Gerwig gegründet. 1947 wurde diese zur Staatlichen Ingenieurschule Furtwangen/Schwarzwald. 1971 wurde diese zur Fachhochschule Furtwangen FHF. 1997 wurde sie zur Hochschule für Technik und Wissenschaft umfirmiert",
            "A"
        ],
        "03_02", [
            "Auf welcher Höhe befindest du dich gerade",
            "~ 90m ü.n.N.",
            "~ 580m ü.n.N.",
            "~ 1100m ü.n.N.",
            "~ 2370m ü.n.N.",
            "m ü.n.N steht für Meter über normal Null",
            "C"
        ],
        "03_03", [
            "Welche Sage gibt es über Studierende der Hochschule Furtwangen?",
            "Wenn mindestens drei mal in der Woche in der Mensa isst, besteht man seine Prüfungen",
            "Wenn Studierende während ihres Studiums das Uhrenmuseum besuchen, schaffen sie ihren Abschluss nicht.",
            "Studierende die im Wohnheim GHB wohnen, werden von den Professor:inn:en bevorzugt.",
            "Die Statue an der Brücke vor dem B-Bau der Hochschule, soll die erste weibliche Absolventin darstellen.",
            "Schon im ersten Semester wird den Studierenden beigebracht, dass sie erst nach ihrem bestandenen Abschluss, das Uhrenmuseum betreten sollten",
            "B"
        ],
        "03_04", [
            "Wie viele Höhenmeter trennen den niedrigsten vom höchsten Punkt der Stadt Furtwangen?",
            "500m",
            "600m",
            "400m",
            "300m",
            "50m (Breg) - 1150m (Brend)",
            "D"
        ],
        "04_01", [
            "Welche Berge kann man neben dem Feldberg und dem Kandel vom Brendturm aus sehen?",
            "Schauinsland",
            "Blickinstal",
            "Sehdasgrün",
            "Spührdasblau",
            "Der Brend ist mit 1148 m der höchste Berg der Stadt Furtwangen, auf dessen Spitze der Brendturm steht. Von seinem Aussichtspunkt aus  17 m Höhe, kann man den Schauinsland sehen. Dieser ist 1284 m hoch und gleichzeitig der Hausberg der Stadt Freiburg. Bei schönem Wetter kann man ringsherum in die schönen Täler schauen.",
            "A"
        ],
        "04_02", [
            "Welche Eselsbrücke gibt es, um sie die Zuflüsse (Brigach und Breg) der Donau zu merken?",
            "Brigach und Breg bringen die Donau zu Weg",
            "Brigach von rechts, Breg von links daraus macht die Donau springs",
            "Zwei Mal B, das tut der Donau nicht weh",
            "Durch Brigach und Bregs Mündung, kommt die Donau zur Verkündung",
            "Länge der Flüsse und wo sie münden.",
            "A"
        ],
        "04_03", [
            "Wie heißt der Größte Berg im Schwarzwald",
            "Mount Everest",
            "Zugspitze",
            "Feldberg",
            "Brend",
            "Der Feldberg ist der höchste Berg im Schwarzwald. Er ist 1493 Meter hoch. Mit der Seilbahn kann man fast bis zum Gipfel fahren.",
            "C"
        ],
        "04_04", [
            "Wieso wurde die Linachtalsperre ursprünglich gebaut?",
            "Zum Schutz vor Überschwemmungen nach der Schneeschmelze",
            "Zur Stromgewinnung",
            "Damit ein Badesee entsteht",
            "Damit eine Örtliche Fischzucht entstehen kann",
            "Die Linachtalsperre wurde von 1922-1925 gebaut. bis 1969 wurde sie, wie ursprünglich geplant, zur örtlichen Stromgewinnung genutzt. Aus Sicherheitsgründen wurde 1988 das Wasser abgelassen. Seit ihrer Renovierung 2007 staut sie wieder das Wasser der Linach.",
            "B"
        ]
    ];


    // for (let i = 0; i < myArray.length; i++) {
    //     if (myArray[i] == questionID) {
    //         document.getElementById('answerA').innerHTML = myArray[i + 1][0];
    //         document.getElementById('answerB').innerHTML = myArray[i + 1][1];
    //         document.getElementById('answerC').innerHTML = myArray[i + 1][2];
    //         document.getElementById('answerD').innerHTML = myArray[i + 1][3];
    //         document.getElementById('explenation').innerHTML = myArray[i + 1][4];
    //     }
    // }


    function selectAnswer(selectedAnswer) {
        selectedAnswerGlobal = selectedAnswer;
        //style Selected Answer
        console.log(selectedAnswer);

        //style unselected answers
        var allAnswers = document.getElementById('answers').children;
        console.log(allAnswers);
    }



    document.getElementById('login-answer').addEventListener('click', function () {
        var id = selectedAnswerGlobal.id;
        console.log(id)
        ajaxCall("/?selectedAnswer=" + id.substr(6));
        window.location.reload();
    });


    function ajaxCall(link) {
        console.log(link);
        var xmlhttp = new XMLHttpRequest();
        xmlhttp.open("GET", link, true);
        xmlhttp.send();
    }
</script>

</html>
)=====";

const char playerIndex[] = R"=====(
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Player</title>

</head>

<body>
    <h3>COde ingeben</h3>

    <input type="text" name="gamecode-input" id="gamecode-input">

    <button id="enter-game">Spiel beitreten</button>
</body>
<script>
    document.getElementById('enter-game').addEventListener('click', function () {
        ajaxCall('/checkKey='+document.getElementById('gamecode-input').value);
        window.location.reload();
    });
    function ajaxCall(link) {
        console.log(link);
        var xmlhttp = new XMLHttpRequest();
        xmlhttp.open("GET", link, true);
        xmlhttp.send();
    }

</script>
<span onload="window.location.assign('')"></span>

</html>

)=====";

const char playerWait[] = R"=====(
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Player Wartem</title>
</head>

<body>
    <p>Game Measter wählt eine Frage aus</p>
</body>
<script>
    const interval = setInterval(function () {
        const xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                if (this.responseText.trim() == "RELOAD") {
                    window.location.reload();
                }
            }
        };
        xhttp.open("GET", "command", true);
        xhttp.send();
    }, 1000);
</script>

</html>
)=====";

const char solutionHTML[] = R"=====(
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Richtige Antwort</title>
</head>

<body>
    <h1>Auflösung</h1>
    <p id="feedback"></p>
    <p id="solution"></p>
    <p id="wait-info">Der Game MAster wäjlt eine Fage aus. Bitte Warten</p>
</body>

<script>
    // const correctAnswerGlobal = "$correctAnswer";
    // const selectedAnswerGlobal = "$selectedAnswer";

    var explenationSet;

    if (!explenationSet) {
        const interval = setInterval(function () {
            const xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function () {
                if (this.readyState == 4 && this.status == 200) {
                    console.log(this.responseText);
                    if (this.responseText.trim() != "") {
                        if (this.responseText.indexOf("RELOAD") < 0) {
                            const json = JSON.parse(this.responseText.trim());

                            if (json.selectedAnswer == json.correctAnswer) {
                                document.getElementById('feedback').innerHTML = 'Deine Antwort ist richtig';
                                ajaxCall("/?result=right");
                                console.log('RICHTIG');
                            } else {
                                document.getElementById('feedback').innerHTML =
                                    'Deine Antwort ist falsch. Die Richtige Antwort ist: ';
                                ajaxCall("/?result=wrong");
                                console.log('FALSCH');
                            }
                            document.getElementById('solution').innerHTML = json.correctAnswer + ': ' +
                                explanations['$QUESTION'];

                            document.selectedAnswer('');
                        } else {
                            console.log("reload response");
                            ajaxCall("/newQuestion");
                            window.location.reload();
                        }
                    }
                }
            }

            xhttp.open("GET", "command", true);
            xhttp.send();
        }, 1000);

        function ajaxCall(link) {
            console.log(link);
            var xmlhttp = new XMLHttpRequest();
            xmlhttp.open("GET", link, true);
            xmlhttp.send();
        }
    }



    const explanations = {
        "01_01": "explanation",
        "01_02": "explanation2",
        "01_03": "explanation",
        "01_04": "explanation",
        "02_01": "explanation",
        "02_02": "explanation",
        "02_03": "explanation",
        "02_04": "explanation",
        "03_01": "explanation",
        "03_02": "explanation",
        "03_03": "explanation",
        "03_04": "explanation",
        "04_01": "explanation",
        "04_02": "explanation",
        "04_03": "explanation",
        "04_04": "explanation"
    }

    // document.getElementById('solution').innerHTML = explanations["01_02"];
</script>


</html>
)=====";

const char playerCheckKey[] = R"=====(
    <!DOCTYPE html><html lang="en"><head><meta charset="UTF-8"><meta http-equiv="X-UA-Compatible" content="IE=edge"><meta name="viewport" content="width=device-width, initial-scale=1.0"><title>Document</title><link rel="stylesheet" href="style.css"></head><body><div class="wrapper"><div class="headerWrapper"><p class="header">Code wird überprüft</p></div></div></body><script>const interval=setInterval(function (){ const xhttp=new XMLHttpRequest(); xhttp.onreadystatechange=function (){ if (this.readyState==4 && this.status==200){ if (this.responseText.trim()=="RELOAD"){ window.location.reload();}}}; xhttp.open("GET", "command", true); xhttp.send();}, 1000);
    </script></html>
)=====";

const char playerWin[] = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Gwonnen</title>
    <link rel="stylesheet" href="style.css">
</head>
<body>
    <h3>Gewommen</h3>
</body>
</html>
)=====";


const char playerLoose[] = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Niederlage</title>
    <link rel="stylesheet" href="style.css">
</head>
<body>
    <h3>Niederlage</h3>
</body>
</html>
)=====";