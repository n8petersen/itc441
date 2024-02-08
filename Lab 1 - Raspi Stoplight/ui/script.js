function redLight() {
    fetch('/api/light/red')
        .then(response => response.json())
        .then(data => {
            console.log(data);
        })
        .catch(error => {
            console.error('Error:', error);
        });

    document.getElementById("color1").checked = true;
    document.getElementById("color2").checked = false;
    document.getElementById("color3").checked = false;

    document.getElementById("off_btn").setAttribute('class', ' btn btn-info')
    document.getElementById("timer_btn").setAttribute('class', ' btn btn-info')
    document.getElementById("timer_btn").setAttribute('onclick', 'timerLight()')
}

function yellowLight() {
    fetch('/api/light/yellow')
        .then(response => response.json())
        .then(data => {
            console.log(data);
        })
        .catch(error => {
            console.error('Error:', error);
        });

    document.getElementById("color1").checked = false;
    document.getElementById("color2").checked = true;
    document.getElementById("color3").checked = false;

    document.getElementById("off_btn").setAttribute('class', ' btn btn-info')
    document.getElementById("timer_btn").setAttribute('class', ' btn btn-info')
    document.getElementById("timer_btn").setAttribute('onclick', 'timerLight()')
}

function greenLight() {
    fetch('/api/light/green')
        .then(response => response.json())
        .then(data => {
            console.log(data);
        })
        .catch(error => {
            console.error('Error:', error);
        });

    document.getElementById("color1").checked = false;
    document.getElementById("color2").checked = false;
    document.getElementById("color3").checked = true;

    document.getElementById("off_btn").setAttribute('class', ' btn btn-info')
    document.getElementById("timer_btn").setAttribute('class', ' btn btn-info')
    document.getElementById("timer_btn").setAttribute('onclick', 'timerLight()')
}

function offLight() {
    fetch('/api/light/off')
        .then(response => response.json())
        .then(data => {
            console.log(data);
        })
        .catch(error => {
            console.error('Error:', error);
        });

    document.getElementById("color1").checked = false;
    document.getElementById("color2").checked = false;
    document.getElementById("color3").checked = false;

    document.getElementById("off_btn").setAttribute('class', ' btn btn-success')
    document.getElementById("timer_btn").setAttribute('class', ' btn btn-info')
    document.getElementById("timer_btn").setAttribute('onclick', 'timerLight()')

}

function timerLight() {
    fetch('/api/light/timer')
        .then(response => response.json())
        .then(data => {
            console.log(data);
        })
        .catch(error => {
            console.error('Error:', error);
        });

    document.getElementById("color1").checked = true;
    document.getElementById("color2").checked = true;
    document.getElementById("color3").checked = true;

    document.getElementById("off_btn").setAttribute('class', ' btn btn-info')
    document.getElementById("timer_btn").setAttribute('class', ' btn btn-success')
    document.getElementById("timer_btn").setAttribute('onclick', '')
}