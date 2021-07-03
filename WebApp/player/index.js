const button = document.querySelector("#enter-game");
const input = document.querySelector("#gamecode-input");
const wrapper = document.querySelector(".wrapper");

let interval;

function clickHandler() {
    clearInterval(interval);
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
                            } else if (this.responseText.trim() == "INVALID") {
                                clearInterval(interval);
                                const div = document.createElement("div");
                                div.classList.add("error-message");
                                div.innerHTML = "Der Code ist nicht richtig. Bitte versuche es erneut.";
                                wrapper.appendChild(div);
                                input.value = "";
                                input.focus();
                                button.addEventListener("click", clickHandler);
                            }
                        }
                    };
                    const key = input.value;
                    xhttp2.open("GET", "validate-key?key=" + key, true);
                    xhttp2.send();
                }, 1000);
            }
        }
    };
    const key = input.value;
    xhttp.open("GET", "submit-key?key=" + key, true);
    xhttp.send();
}
button.addEventListener("click", clickHandler);