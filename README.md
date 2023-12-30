<h1 align="center">HX711 Library for STM32</h1>

- HAL library uses

<h2>How to use:</h2>

<h3>INIT:</h3>

    hx711_init(HX711_DT_GPIO_Port, HX711_DT_Pin, HX711_SCK_GPIO_Port, HX711_SCK_Pin);

    hx711_turnOn();
    hx711_tara(5);

<h3>READ:</h3>

    weight = hx711_getWeight();

## :memo: License ##
This project is licensed under the MIT License. For more details, please refer to the [LICENSE](LICENSE.md) file.

<br/>
<p align="center">Made by <a href="https://github.com/kosik-prog/" target="_blank">KoSik</a><p/>
<br/>
<a href="#top">Back to top</a>