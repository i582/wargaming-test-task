# Тестовое задание на позицию Programming intern в компании Wargaming Санкт-Петербург

## Задание 1


<details>
  <summary>Условие задания</summary>

    На языке Python или С/С++, написать алгоритм (функцию) определения четности целого числа, который будет аналогичен нижеприведенному по функциональности, но отличен по своей сути.
    
    Объяснить плюсы и минусы обеих реализаций.
    
    Python example:
    ```python
    def isEven(value):
       return value % 2 == 0
    ```
    C/C++ example:
    ```cpp
    bool isEven(int value)
    {
       return value % 2 == 0;
    }
    ```

</details>


С++:

​	1. Исходный код: [ссылка](https://github.com/i582/wargaming-test-task/blob/master/cpp/src/first/is_even.h)



## Задание 2

<details>
  <summary>Условие задания</summary>

    На языках Python(2.7) и/или С++, написать минимум по 2 класса реализовывающих циклический буфер FIFO.
    
    Объяснить плюсы и минусы каждой реализации.

</details>

С++:

1. Реализация через индексы: [ссылка](https://github.com/i582/wargaming-test-task/blob/master/cpp/src/second/cyclic_queue.h);
2. Реализация с помощью связных списков: [ссылка](https://github.com/i582/wargaming-test-task/blob/master/cpp/src/second/cyclic_queue_list.h).

У 1 варианта выигрыш в памяти, так как не нужно хранить указатель на следующий элемент связного списка. У 2 варианта выигрыш за счет более быстрого перехода после добавления/удаления на следующий элемент (Одно присваивание вместо присваивания, сложения и одной проверки)

Python:

1. Реализация через индексы: [ссылка](https://github.com/i582/wargaming-test-task/blob/master/python/second/main.py).



## Задание 3

<details>
  <summary>Условие задания</summary>

    На языке Python или С/С++, написать функцию, которая быстрее всего (по процессорным тикам) отсортирует данный ей массив чисел.
    
    Массив может быть любого размера со случайным порядком чисел (в том числе и отсортированным).
    
    Объяснить почему вы считаете, что функция соответствует заданным критериям.

</details>

В Python по стандарту используется сортировка `Timsort`, когда, как стандарт С++ не устанавливает конкретной реализации `std::sort`, но для примера можно взять `GCC`, который использует `Quicksort`с переходом в пирамидальную. 

Эти две сортировки по праву являются наибыстрейшими. Если рассмотреть графики:

![](https://habrastorage.org/storage1/a0170f65/54afe800/d8b50571/a81000af.png)

![](https://habrastorage.org/storage1/d0460884/b7d712b4/cd04f5c3/4c115b90.png)

![](https://habrastorage.org/storage1/fd520aa1/8be42114/57e27349/d01105c5.png)

![](https://habrastorage.org/storage1/0ac3016e/f3ce8871/2bf28d22/cb5cd3f2.png)

То можно заметить, что на сырых данных `Quicksort` выигрывает `Timsort`, но в тоже время, как только данные становятся отчасти отсортированными, то `Timsort` начинает обгонять быструю сортировку. Также стоит сказать, что реализация быстрой сортировки намного проще реализации `Timsort`, поэтому для простых проектов на С++ использование `Timsort` выглядит излишеством, в то время как `std::sort` уже есть в стандартной библиотеке. В случае же проектов, где необходимо сортировать огромные частично упорядоченные данные, `Timsort` будет отличным выбором. Поэтому для простых проектов С++, я бы использовал стандартный `std::sort`, а в случае `python` встроенный `sort` с `Timsort`. 