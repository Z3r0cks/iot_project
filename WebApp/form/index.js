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