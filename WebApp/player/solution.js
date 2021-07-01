const rawQuestion = `$QUESTION`;
const header = document.querySelector(".header");
const feedback = document.querySelector("#feedback");
const solution = document.querySelector("#solution");
const correct = "$CORRECT";
const answer = "$ANSWER";

if (rawQuestion)
    header.innerHTML = "Auflösung";
else
    header.innerHTML = "Warte auf Frage";

if (rawQuestion) {
    question = JSON.parse(rawQuestion);
    const text = `Du hast ${answer} gewählt. Das ist ${answer == correct ? "richtig" : "falsch"}.`
    feedback.innerHTML = text;
    solution.innerHTML = question.explanation;
}

const interval = setInterval(function () {
    const xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            if (this.responseText.trim() == "$VALIDATION") {
                clearInterval(interval);
                window.location.reload();
            }
        }
    };
    xhttp.open("GET", "await-question", true);
    xhttp.send();
}, 1000);