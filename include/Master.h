const char questionArray[] = R"=====(
const questions = {
    "01_01": {
        question: "Frage",
        answers: ["Antwort A", "Antwort B", "Antwort C", "Antwort D"],
        category: 1
    },
    "01_02": {
        question: "Frage",
        answers: ["Antwort A", "Antwort B", "Antwort C", "Antwort D"],
        category: 1
    },
    "01_03": {
        question: "Frage",
        answers: ["Antwort A", "Antwort B", "Antwort C", "Antwort D"],
        category: 1
    },
    "01_04": {
        question: "Frage",
        answers: ["Antwort A", "Antwort B", "Antwort C", "Antwort D"],
        category: 1
    },
    "02_01": {
        question: "Frage",
        answers: ["Antwort A", "Antwort B", "Antwort C", "Antwort D"],
        category: 2
    },
    "02_02": {
        question: "Frage",
        answers: ["Antwort A", "Antwort B", "Antwort C", "Antwort D"],
        category: 2
    },
    "02_03": {
        question: "Frage",
        answers: ["Antwort A", "Antwort B", "Antwort C", "Antwort D"],
        category: 2
    },
    "02_04": {
        question: "Frage",
        answers: ["Antwort A", "Antwort B", "Antwort C", "Antwort D"],
        category: 2
    },
    "03_01": {
        question: "Frage",
        answers: ["Antwort A", "Antwort B", "Antwort C", "Antwort D"],
        category: 3
    },
    "03_02": {
        question: "Frage",
        answers: ["Antwort A", "Antwort B", "Antwort C", "Antwort D"],
        category: 3
    },
    "03_03": {
        question: "Frage",
        answers: ["Antwort A", "Antwort B", "Antwort C", "Antwort D"],
        category: 3
    },
    "03_04": {
        question: "Frage",
        answers: ["Antwort A", "Antwort B", "Antwort C", "Antwort D"],
        category: 3
    },
    "04_01": {
        question: "Frage",
        answers: ["Antwort A", "Antwort B", "Antwort C", "Antwort D"],
        category: 4
    },
    "04_02": {
        question: "Frage",
        answers: ["Antwort A", "Antwort B", "Antwort C", "Antwort D"],
        category: 4
    },
    "04_03": {
        question: "Frage",
        answers: ["Antwort A", "Antwort B", "Antwort C", "Antwort D"],
        category: 4
    },
    "04_04": {
        question: "Frage",
        answers: ["Antwort A", "Antwort B", "Antwort C", "Antwort D"],
        category: 4
    }
}
)=====";

const char explanationArray[] = R"=====(
const explanations = {
    "01_01": "explanation",
    "01_02": "explanation",
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
)=====";

const char masterIndex[] = R"=====(
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Master Frage</title>
    <script src="index.js" defer></script>
    <link rel="stylesheet" href="style.css">
</head>

<body>
    <h3>Code</h3>
    <h1 id="gamecode">----</h1>
</body>

</html>
)=====";

const char masterIndexJS[] = R"=====(
document.querySelector("#gamecode").innerHTML="$KEY";
const interval = setInterval(function () {
    const xhttp = new XMLHttpRequest(); 
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            if(this.responseText.trim() == "$VALIDATION") {
                clearInterval(interval);
                window.location.reload();
            }
        }
    };
    xhttp.open("GET", "validate-key", true); 
    xhttp.send(); 
}, 1000);
)=====";

const char masterQuestions[] = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Master Frage</title>
    <script src="questionArray.js" defer></script>
    <script src="questions.js" defer></script>
    <link rel="stylesheet" href="style.css">
</head>
<body>
    <div id="cat1"></div>
    <div id="cat2"></div>
    <div id="cat3"></div>
    <div id="cat4"></div>
</body>
</html>
)=====";

const char masterQuestionsJS[] = R"=====(
for (const key in questions) {
    const q = questions[key];
    const div = document.createElement("div");
    div.classList.add("question");
    div.id = key;
    const text = document.createElement("div");
    text.innerHTML = q.question;
    div.appendChild(text);
    for (const answer of q.answers) {
        const a = document.createElement("a");
        a.innerHTML = answer;
        div.appendChild(a);
    }
    // EVENT LISTENER
    function clickHandler() {
        const xhttp = new XMLHttpRequest(); 
        xhttp.onreadystatechange = function() {
            if (this.readyState == 4 && this.status == 200) {
                if(this.responseText.trim() == "$VALIDATION") {
                    div.removeEventListener("click", clickHandler);
                    window.location.reload();
                }
            }
        };
        xhttp.open("GET", "choose-question?question=" + key, true); 
        xhttp.send(); 
    }
    div.addEventListener("click", clickHandler);
    document.querySelector("#cat" + questions[key].category).appendChild(div);
}
)=====";

const char masterSolution[] = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Richtige Antwort</title>
    <script src="questionArray.js" defer></script>
    <script src="explanationArray.js" defer></script>
    <script src="solution.js" defer></script>
    <link rel="stylesheet" href="style.css">
</head>
<body>
    <p id="solution"></p>
    <p id="wait-info"></p>
</body>
</html>
)=====";

const char masterSolutionJS[] = R"=====(
document.querySelector("#solution").innerHTML = questions["$ID"].question;
document.querySelector("#wait-info").innerHTML = explanations["$ID"];
const interval = setInterval(function () {
    const xhttp = new XMLHttpRequest(); 
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            if(this.responseText.trim() == "$VALIDATION") {
                clearInterval(interval);
                window.location.reload();
            }
        }
    };
    xhttp.open("GET", "validate-answer", true); 
    xhttp.send(); 
}, 1000);
)=====";

const char masterVictory[] = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Sieg</title>
    <link rel="stylesheet" href="style.css">
</head>
<body>
    <h3>Sieg</h3>
</body>
</html>
)=====";

const char masterDefeat[] = R"=====(
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

const char masterError[] = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Fehler</title>
    <link rel="stylesheet" href="style.css">
</head>
<body>
    <h3>Ups!</h3>
    <p>Etwas ist schief gelaufen.<p>
</body>
</html>
)=====";