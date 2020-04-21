function httpRequst(url, changedElement, positiveFeedback, negativeFeedback) {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200)
        {
            document.getElementById(changedElement).innerHTML = positiveFeedback;
            textToDisplay = "";
            if(-1 != this.responseText.search("1"))
            {
              textToDisplay = positiveFeedback;
            }
            else
            {
                textToDisplay = negativeFeedback;
            }
            document.getElementById(changedElement).innerHTML = textToDisplay;
        }
      };
      xhttp.open("GET", url, true);
      xhttp.send();
    }
    function httpRequestSync(url)
    {
        var request = new XMLHttpRequest();
        request.open('GET', url, false);  // `false` makes the request synchronous
        request.send(null);
        if (request.status === 200) {
            return request.responseText;
        }
        return "";
    }
    function checkEmail(value, emailResponseId)
    {
        if(5 > value.length)
        {
            return "Email is too short!";
        }
        if((-1 == value.search("@"))
            || (-1 == value.search(".")))
        {
            return "Invalid email!";
        }
        httpRequst("http://localhost/checkEmail.php?Email="+value, emailResponseId, "Email already exists!", "");
        return "";
    }
    function doesContainUpper(value)
    {
        for(var i = 0; i < value.length; i++)
        {
            if('A' <= value.charAt(i)
                && 'Z' >= value.charAt(i))
            {
                return 1;
            }
        }
        return 0;
    }
    function doesContainLower(value)
    {
        for(var i = 0; i < value.length; i++)
        {
            if('a' <= value.charAt(i)
                && 'z' >= value.charAt(i))
            {
                return 1;
            }
        }
        return 0;
    }
    function doesContainNumber(value)
    {
        for(var i = 0; i < value.length; i++)
        {
            if('0' <= value.charAt(i)
                && '9' >= value.charAt(i))
            {
                return 1;
            }
        }
        return 0;
    }
    function checkPassword(value)
    {
        if(5 > value.length)
        {
            return "Password is not complex enough!";
        }
        if(!doesContainUpper(value))
        {
            return "The password must contain at least one upper character";
        }
        if(!doesContainLower(value))
        {
            return "The password must contain at least one lower character";
        }
        if(!doesContainNumber(value))
        {
            return "The password must contain at least one number";
        }
        return "";
    }
