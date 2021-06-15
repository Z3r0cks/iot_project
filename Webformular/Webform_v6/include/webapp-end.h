const char webappEnd[] = R"=====(
        <script>
        function validateForm() {
            console.log('validate')

            var xmlhttp = new XMLHttpRequest();
            xmlhttp.open("GET", "/save/__!q" + document.forms["form"]["question"].value +
                "__!a" + document.forms["form"]["answerA"].value +
                "__!b" + document.forms["form"]["answerB"].value +
                "__!c" + document.forms["form"]["answerC"].value +
                "__!d" + document.forms["form"]["answerD"].value, true);
            xmlhttp.send();
        }

        function deleteSPIFFS() {
            console.log("Löschen");
            var xmlhttp = new XMLHttpRequest();
            xmlhttp.open("GET", "/delete", true);
            xmlhttp.send();
        }

        function formatContent(content) {
            content = content.replace('__F:', '\n\nF:');
            content = content.replace('__A:', '\nA:');
            content = content.replace('__B:', '\nB:');
            content = content.replace('__C:', '\nC:');
            content = content.replace('__D:', '\nD:');
            return content;
        }

        function getFirst(url, toReplace) {
            return url.substring(0, url.indexOf(toReplace));
        }

        function getLast(url, toReplace) {
            return url.substring(url.indexOf(toReplace) + toReplace.length, url.length);
        }

        function encodeOneChar(url, toReplace, newStr) {
            return getFirst(url, toReplace) + newStr + getLast(url, toReplace);
        }

        function encodeOneCharType(str, toReplace, newStr) {
            while (str.indexOf(toReplace) >= 0) {
                str = encodeOneChar(str, toReplace, newStr);
            }
            return str;
        }

        function encodeFullString(str) {
            str = encodeOneCharType(str, "%C3%9F", "ß");
            str = encodeOneCharType(str, "%C3%A4", "Ä");
            str = encodeOneCharType(str, "%C3%84", "ä");
            str = encodeOneCharType(str, "%C3%B6", "Ö");
            str = encodeOneCharType(str, "%C3%96", "ö");
            str = encodeOneCharType(str, "%C3%BC", "Ü");
            str = encodeOneCharType(str, "%C3%9C", "ü");
            str = encodeOneCharType(str, "%20", " ");
            str = encodeOneCharType(str, "%21", "!");
            str = encodeOneCharType(str, "%22", "\"");
            str = encodeOneCharType(str, "%23", "#");
            str = encodeOneCharType(str, "%24", "$");
            str = encodeOneCharType(str, "%25", "%");
            str = encodeOneCharType(str, "%26", "&");
            str = encodeOneCharType(str, "%27", "'");
            str = encodeOneCharType(str, "%28", "(");
            str = encodeOneCharType(str, "%29", ")");
            str = encodeOneCharType(str, "%2A", "*");
            str = encodeOneCharType(str, "%2B", "+");
            str = encodeOneCharType(str, "%2C", ",");
            str = encodeOneCharType(str, "%2D", "-");
            str = encodeOneCharType(str, "%2E", ".");
            str = encodeOneCharType(str, "%2F", "/");
            str = encodeOneCharType(str, "%3A", ":");
            str = encodeOneCharType(str, "%3B", ";");
            str = encodeOneCharType(str, "%3C", "<");
            str = encodeOneCharType(str, "%3D", "=");
            str = encodeOneCharType(str, "%3E", ">");
            str = encodeOneCharType(str, "%3F", "?");
            str = encodeOneCharType(str, "%40", "@");
            str = encodeOneCharType(str, "%5B", "[");
            str = encodeOneCharType(str, "%5D", "]");
            str = encodeOneCharType(str, "%7B", "{");
            str = encodeOneCharType(str, "%7C", "|");
            str = encodeOneCharType(str, "%7D", "}");
            return str;
        }

        function createLink(content) {
            if (content.length > 0) {
                var element = document.createElement('a');
                content = encodeFullString(content);
                var end = content.match(/__F/g).length;
                for (let i = 0; i < end; i++) {
                    content = formatContent(content);
                }

                element.setAttribute('href', 'data:text/plain;charset=utf-8,' + encodeURIComponent(content));
                element.setAttribute('download', 'WanderschildFragen');

                element.style.display = 'none';
                document.body.appendChild(element);

                element.click();

                document.body.removeChild(element);
            } else
                alert('Es wurde keine Fragen eingereicht')
        }
    </script>

</body></html>
)=====";