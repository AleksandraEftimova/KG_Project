# Проект по Компјутерска Графика
# Игра налик на "Temple Run"

## 1. Вовед
Целта на овој проект е да се демонстрира игра налик на Temple Run, односно игра каде главниот фокус би бил на движење по патека и соодветна ротација во зависност од влезни команди.

## 2. Чекори и начин на изработка
При изработка на овој проект користени беа следните библиотеки: GLAD, GLFW, GLM, STB.

Чекори:
1) Креирање на нов проект и успешно компајлирање на почетниот код (празен прозорец, "Hello Window").
2) Поставување на почетни координати и креирање на патеката.
3) Додавање атрибути на првично-дефинираните позиции со кои програмата би знаела како да ја постави текстурата. За добивање на саканиот резултат задолжително мораа да се креираат Vertex Shader и Fragment Shader фајлови, напишани во GLSL.
4) Поставување на камерата, односно:
    - нејзина почетна позиција
    - како да биде поставена во просторот (која оска да биде кон напред, која за нагоре)
    - поставување цело време да се движи кон напред
    - дефинирање брзина со која се движи
    - дефинирање чувствителност (sensitivity) при ротација или движење на камерата
5) Поставување на влезни команди преку кои би ја движеле и вртеле камерата (лева стрелка од тастатура = вртење налево, десна стрелка од тастатура = вртење надесно, буква А = движење во лево, буква D = движење во десно)

## 3. Користен материјал
  - https://learnopengl.com/
  - https://github.com/joksim/OpenGLPrj

## 4. Линк до изворен код
  https://github.com/AleksandraEftimova/KG_Project
