### Визуализатор бинарных деревьев поиска

+ АВЛ, красно-чёрное, Splay
+ Масштабирование
+ Логарифмическая шкала масштабирования


<p align="center"><img src="demo.gif"  alt="demo"/></p>


#### Сборка:

Минимальная версия Qt, на которой проект был проверен:  5.11.3


Удобным для вас споcобом передайте cmake ключ содержащий путь до Qt:

`-DCMAKE_PREFIX_PATH=/YOUR_QT_PATH/Qt/YOUR_QT_VERSION/clang_64/lib/cmake`

Рекомендую использовать CLion для сборки

Известные проблемы:

+ Случайные баги при масштабировании

+ При управлении с тачпада ползунок масштабирования случайным образом отскакивает от краёв

+ Нахлёст некоторых элементов при использовании различных версий Qt 