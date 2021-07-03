const char playerIndexJS[] = R"=====(
"use strict";
var button = document.querySelector("#enter-game");
var input = document.querySelector("#gamecode-input");
var interval;
function clickHandler() {
    clearInterval(interval);
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            if (this.responseText.trim() == "$VALIDATION") {
                button.removeEventListener("click", clickHandler);
                interval = setInterval(function () {
                    var xhttp2 = new XMLHttpRequest();
                    xhttp2.onreadystatechange = function () {
                        if (this.readyState == 4 && this.status == 200) {
                            if (this.responseText.trim() == "$VALIDATION") {
                                clearInterval(interval);
                                window.location.reload();
                            }
                            else if (this.responseText.trim() == "INVALID") {
                                clearInterval(interval);
                                var div = document.createElement("div");
                                div.classList.add("error-message");
                                div.innerHTML = "Der Code ist nicht richtig. Bitte versuche es erneut.";
                                document.body.appendChild(div);
                                input.value = "";
                                input.focus();
                                button.addEventListener("click", clickHandler);
                            }
                        }
                    };
                    var key = input.value;
                    xhttp2.open("GET", "validate-key?key=" + key, true);
                    xhttp2.send();
                }, 1000);
            }
        }
    };
    var key = input.value;
    xhttp.open("GET", "submit-key?key=" + key, true);
    xhttp.send();
}
button.addEventListener("click", clickHandler);
)=====";

const char playerSolutionJS[] = R"=====(
"use strict";
var rawQuestion = "$QUESTION";
var header = document.querySelector(".header");
var feedback = document.querySelector("#feedback");
var solution = document.querySelector("#solution");
var correct = "$CORRECT";
var answer = "$ANSWER";
if (rawQuestion)
    header.innerHTML = "Auflösung";
else
    header.innerHTML = "Warte auf Frage";
if (rawQuestion) {
    var quest = JSON.parse(rawQuestion);
    var text = "Du hast " + answer + " gew\u00E4hlt. Das ist " + (answer == correct ? "richtig" : "falsch") + ".";
    feedback.innerHTML = text;
    solution.innerHTML = quest.explanation;
}
var interval = setInterval(function () {
    var xhttp = new XMLHttpRequest();
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
)=====";

const char playerQuestionJS[] = R"=====(
"use strict";
var question = $QUESTION;
var text = document.querySelector("#question");
var a = document.querySelector("#answerA");
var b = document.querySelector("#answerB");
var c = document.querySelector("#answerC");
var d = document.querySelector("#answerD");
var button = document.querySelector("#login-answer");
var selected;
text.innerHTML = question.text;
a.innerHTML = question.answers[0];
b.innerHTML = question.answers[1];
c.innerHTML = question.answers[2];
d.innerHTML = question.answers[3];
a.dataset.value = "A";
b.dataset.value = "B";
c.dataset.value = "C";
d.dataset.value = "D";
function clickHandler(event) {
    selected = event.target.dataset.value;
}
var interval;
function submitHandler() {
    if (selected) {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                if (this.responseText.trim() == "$VALIDATION") {
                    button === null || button === void 0 ? void 0 : button.removeEventListener("click", clickHandler);
                    interval = setInterval(function () {
                        var xhttp2 = new XMLHttpRequest();
                        xhttp2.onreadystatechange = function () {
                            if (this.readyState == 4 && this.status == 200) {
                                if (this.responseText.trim() == "$VALIDATION") {
                                    clearInterval(interval);
                                    window.location.reload();
                                }
                            }
                        };
                        xhttp2.open("GET", "validate-answer" + selected, true);
                        xhttp2.send();
                    }, 1000);
                }
            }
        };
        xhttp.open("GET", "submit-answer?answer=" + selected, true);
        xhttp.send();
    }
}
a.addEventListener("click", clickHandler);
b.addEventListener("click", clickHandler);
c.addEventListener("click", clickHandler);
d.addEventListener("click", clickHandler);
button === null || button === void 0 ? void 0 : button.addEventListener("click", submitHandler);
)=====";