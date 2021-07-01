const question = $QUESTION;
const text = document.querySelector("#question");
const a = document.querySelector("#answerA");
const b = document.querySelector("#answerB");
const c = document.querySelector("#answerC");
const d = document.querySelector("#answerD");
const button = document.querySelector("#login-answer");

let selected;

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

let interval;

function submitHandler() {
    if (selected) {
        const xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                if (this.responseText.trim() == "$VALIDATION") {
                    button.removeEventListener("click", clickHandler);
                    interval = setInterval(function () {
                        const xhttp2 = new XMLHttpRequest();
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
button.addEventListener("click", submitHandler);