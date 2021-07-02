const char masterIndexJS[] = R"=====(
document.querySelector("#gamecode").innerHTML = "$KEY";
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
    xhttp.open("GET", "validate-key", true);
    xhttp.send();
}, 1000);
)=====";

const char masterQuestionsJS[] = R"=====(
const questions = $QUESTIONS;
for (const key in questions) {
    const quest = questions[key];
    const div = document.createElement("div");
    div.classList.add("question");
    div.id = key;
    
    div.innerHTML = quest.text;
    function clickHandler() {
        const xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                if (this.responseText.trim() == "$VALIDATION") {
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

const char masterSolutionJS[] = R"=====(
const questions = $QUESTIONS;
document.querySelector("#solution").innerHTML = questions["$ID"].text;
document.querySelector("#wait-info").innerHTML = questions["$ID"].explanation;
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
    xhttp.open("GET", "validate-answer", true);
    xhttp.send();
}, 1000);
)=====";

const char formIndexJS[] = R"=====(
const raw = {};
const button = document.querySelector("#submit");
const question = document.querySelector("#question");
const a = document.querySelector("#answerA");
const b = document.querySelector("#answerB");
const c = document.querySelector("#answerC");
const d = document.querySelector("#answerD");
const form = document.querySelector("#form");

function validate() {
    raw.question = question.value;
    raw.a = a.value;
    raw.b = b.value;
    raw.c = c.value;
    raw.d = d.value;
    return true;
    return raw.question && raw.a && raw.b && raw.c && raw.d;
}

function submitHandler(event) {
    if (validate()) {
        const xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                if (this.responseText.trim() == "$VALIDATION") {
                    button.removeEventListener("click", submitHandler);
                    button.disabled = true;
                }
            }
        };
        const data = `${raw.question};${raw.a};${raw.b};${raw.c};${raw.d}`;
        xhttp.open("GET", `submit-question?data=${btoa(JSON.stringify(data))}&end=true`, true);
        xhttp.send();
    } else {

    }
}
button.addEventListener("click", submitHandler);
)=====";