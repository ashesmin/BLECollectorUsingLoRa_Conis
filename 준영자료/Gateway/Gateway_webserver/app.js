var express = require('express'),
    body_parser = require('body-parser'),
    fs = require('fs'),
    exec = require('child_process').exec,
    gateway, pid;
var app = express();
app.use(body_parser.json());
app.use(express.static(__dirname + '/public'));
app.use('/css', express.static(__dirname + '/node_modules/bootstrap/dist/css'));
app.use('/js', express.static(__dirname + '/node_modules/bootstrap/dist/js'));

pid = 0;

app.get('/systemtime', function(req, res) {

    var currentDate = new Date();
    var dateTime = " System Clock " + currentDate.getFullYear() + "/" +
        (currentDate.getMonth() + 1) + "/" +
        currentDate.getDate() + "  " +
        currentDate.getHours() + ":" +
        currentDate.getMinutes() + ":" +
        currentDate.getSeconds();

    res.send(dateTime.toString());
});


app.post('/exps', function(req, res) {
    if (pid > 0) {
        res.send("아직 진행중인 실험이 있습니다.");
    } else {
        //.cfg 파일 생성
        var expData = req.body;
        console.log('Config data: ' + JSON.stringify(expData));

        var file = '/home/pi/LoFiEXP/Gateway/usermode.cfg';
        fs.open(file, 'w+', function(err, fd) {
            if (err) throw err;
            console.log('file open complete');
        });

        fs.writeFile(file, JSON.stringify(expData), function(err) {
            console.log('write end');
        });

        
        gateway = exec('sudo nohup ./expMain.cpp_exe > console.log &', {
            cwd: '/home/pi/LoFiEXP/Gateway'
        }, function(err, stdout, stderr) {
            console.log('stdout: ' + stdout);
            console.log('stderr: ' + stderr);
            if (err !== null) {
                console.log('exec error: ' + err);
            }
            pid = 0;
            console.log("jin tian tian qi hen hao!");
        });

        pid = gateway.pid + 1;

        console.log("pid = " + pid)
        res.send("실험을 시작합니다.");
    }

    res.end();
});


app.get('/exps', function(req, res) {
    if (pid <= 0) {
        res.send("현재 진행중인 실험이 없습니다.");
    } else {
        var kill_cmd = "sudo kill " + pid;

        kill = exec(kill_cmd, function(err, stdout, stderr) {
            //console.log('stdout: ' + stdout);
            console.log('stderr: ' + stderr);
            if (err !== null) {
                console.log('exec error: ' + err);
            }
            //res.send("실험을 중지하였습니다.");
        });

        res.send("실험을 중지하였습니다.");
    }
    res.end();
})

app.put('/exps', function(req, res) {
    var time_obj = req.body;
    var time_cmd = "sudo date -s " + JSON.stringify(time_obj.Time);

    time = exec(time_cmd, function(err, stdout, stderr) {
        console.log('stdout: ' + stdout);
        console.log('stderr: ' + stderr);
        if (err !== null) {
            console.log('exec error: ' + err);
        }
    });

    res.end();
})


var WebSocket = require("ws")

app.listen(4000, function() {
    console.log('Server listening on', 4000);
});

var ws = new WebSocket("ws://192.168.1.1:3000");
ws.onerror = function() {
        console.log("NTP Server Not Connection" );
    }

ws.onopen = function(event) {
    ws.send("Gateway#2");
    //  console.log(JSON.stringify(expArray));

    ws.onmessage = function(event) {
        console.log("Server message: ", event.data);

        var file = '/home/pi/LoFiEXP/Gateway/usermode.cfg';
        fs.open(file, 'w+', function(err, fd) {
            if (err) throw err;
            console.log('file open complete');
        });

        if (pid > 0) {
            if (pid <= 0) {
                res.send("현재 진행중인 실험이 없습니다.");
            } else {
                if (event.data == "Exp Stop") {
                    var kill_cmd = "sudo kill " + pid;

                    kill = exec(kill_cmd, function(err, stdout, stderr) {
                        //console.log('stdout: ' + stdout);
                        console.log('stderr: ' + stderr);
                        if (err !== null) {
                            console.log('exec error: ' + err);
                        }
                        //실험중지
                    });


                }
            }
        } else {

            if (IsJsonString(event.data)) {
                fs.writeFile(file, event.data, function(err) {
                    console.log('write end');
                    gateway = exec('sudo nohup ./expMain.cpp_exe > console.log &', {
                        cwd: '/home/pi/LoFiEXP/Gateway'
                    }, function(err, stdout, stderr) {
                        console.log('stdout: ' + stdout);
                        console.log('stderr: ' + stderr);
                        if (err !== null) {
                            console.log('exec error: ' + err);
                        }
                        pid = 0;

                    });
                    pid = gateway.pid + 1
                    console.log("----EXP Start----");
                });
            }

        }

    }
 
}


function IsJsonString(str) {
    try {
        JSON.parse(str);
    } catch (e) {
        return false;
    }
    return true;
}