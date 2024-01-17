// 温度関数
tem = [];
hum = [];

max = 20;

ids = [];

function looping() {
    fetch ("./get-graphic")
        .then (response=>response.json ())
        .then (data=>{
            if (tem.length == 0 || (tem[tem.length - 1] + 5 > data["tem"] && tem[tem.length - 1] - 5 < data["tem"])){
                tem.push (data["tem"]);
            }
            if (hum.length == 0 || (hum[hum.length - 1] + 5 > data["hum"] && hum[hum.length - 1] - 5 < data["hum"])){
                hum.push (data["hum"]);
            }
            
            if (tem.length > max){
                tem.shift ();
            }
            if (hum.length > max){
                hum.shift ();
            }
        })
        .catch (error=>{
            console.error("APIの取得に失敗しました:", error);
        });

    setTimeout(looping, 1000);
}

looping ();

function tem_graphic() {
    var class_name = document.getElementsByClassName("tem_graphic");

    if (class_name.length !== 1) {
        // 取得したいクラス名を取得することに失敗しました
        console.error("取得したいクラス名を取得することに失敗しました (tem_graphic)");
        console.error("class_name.length => " + class_name.length);
        return;
    }

    var v1 = class_name[0];

    function updateTemperature() {
        const canvas = document.createElement("canvas");

        canvas.width = 40;  // グラフの幅を40に設定
        canvas.height = 30; // グラフの高さを30に設定

        v1.innerHTML = "";
        v1.appendChild(canvas);

        var line_chart = new Chart(canvas, {
            type: "line",
            data: {
                labels: Array.from({ length: tem.length }, (_, i) => i),
                datasets: [
                    {
                        label: "気温 (度)",
                        data: tem,
                        borderColor: "#ec4343",
                        backgroundColor: "#00000000",
                        lineTension: 0
                    }
                ],
            },
            options: {
                title: {
                    display: true,
                    text: "気温情報"
                },
                scales: {
                    yAxes: [{
                        ticks: {
                            suggestedMax: Math.max.apply (null, tem) + 1,
                            suggestedMin: Math.min.apply (null, tem) - 1,
                            stepSize: 0.5,
                            callback: function (value, index, values) {
                                return value + "度";
                            }
                        }
                    }]
                },
                animation: {
                    duration: 0 // アニメーションの時間をゼロに設定
                }
            }
        });

        
        // 次の更新を1秒後にスケジュール
        setTimeout(updateTemperature, 1000);
    }

    // 最初の呼び出し
    updateTemperature();
}

// 湿度関数
function hum_graphic (){
    var class_name = document.getElementsByClassName("hum_graphic");

    if (class_name.length !== 1) {
        // 取得したいクラス名を取得することに失敗しました
        console.error("取得したいクラス名を取得することに失敗しました (hum_graphic)");
        console.error("class_name.length => " + class_name.length);
        return;
    }

    var v1 = class_name[0];

    function updateTemperature() {
        const canvas = document.createElement("canvas");

        canvas.width = 40;  // グラフの幅を40に設定
        canvas.height = 30; // グラフの高さを30に設定

        v1.innerHTML = "";
        v1.appendChild(canvas);

        var line_chart = new Chart(canvas, {
            type: "line",
            data: {
                labels: Array.from({ length: hum.length }, (_, i) => i),
                datasets: [
                    {
                        label: "湿度 (%)",
                        data: hum,
                        borderColor: "#44444444",
                        backgroundColor: "#11111111",
                        lineTension: 0
                    }
                ],
            },
            options: {
                title: {
                    display: true,
                    text: "湿度情報"
                },
                scales: {
                    yAxes: [{
                        ticks: {
                            suggestedMax: Math.max.apply (null, hum) + 1,
                            suggestedMin: Math.min.apply (null, hum) - 1,
                            stepSize: 0.5,
                            callback: function (value, index, values) {
                                return value + "%";
                            }
                        }
                    }]
                },
                animation: {
                    duration: 0 // アニメーションの時間をゼロに設定
                }
            }
        });

        // 次の更新を1秒後にスケジュール
        setTimeout(updateTemperature, 1000);
    }

    // 最初の呼び出し
    updateTemperature();
}

// 全体関数
function all_graphic (){
    var class_name = document.getElementsByClassName("all_graphic");

    if (class_name.length !== 1) {
        // 取得したいクラス名を取得することに失敗しました
        console.error("取得したいクラス名を取得することに失敗しました (all_graphic)");
        console.error("class_name.length => " + class_name.length);
        return;
    }

    var v1 = class_name[0];

    function updateTemperature() {
        const canvas = document.createElement("canvas");

        canvas.width = 40;  // グラフの幅を40に設定
        canvas.height = 30; // グラフの高さを30に設定

        v1.innerHTML = "";
        v1.appendChild(canvas);

        var line_chart = new Chart(canvas, {
            type: "line",
            data: {
                labels: Array.from({ length: tem.length }, (_, i) => i),
                datasets: [
                    {
                        label: "気温 (度)",
                        data: tem,
                        borderColor: "#ec4343",
                        backgroundColor: "#00000000",
                        lineTension: 0
                    },
                    {
                        label: "湿度 (%)",
                        data: hum,
                        borderColor: "#44444444",
                        backgroundColor: "#11111111",
                        lineTension: 0
                    }
                ],
            },
            options: {
                title: {
                    display: true,
                    text: "全体情報"
                },
                scales: {
                    yAxes: [
                        {
                            ticks: {
                                suggestedMax: 100,
                                suggestedMin: 0,
                                stepSize: 10,
                                callback: function (value, index, values) {
                                    return value + "度";
                                }
                            }
                        },
                        {
                            ticks: {
                                suggestedMax: 100,
                                suggestedMin: 0,
                                stepSize: 10,
                                callback: function (value, index, values) {
                                    return value + "%";
                                }
                            }
                        }
                    ]
                },
                animation: {
                    duration: 0 // アニメーションの時間をゼロに設定
                }
            }
        });

        // 次の更新を1秒後にスケジュール
        setTimeout(updateTemperature, 1000);
    }

    // 最初の呼び出し
    updateTemperature();
}

window.onload = function (){
    tem_graphic ();
    hum_graphic ();
    all_graphic ();
};
