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



### Плюсы и минусы

Сравниваем варианты:

1. ```cpp
   bool isEven(int value)
   {
      return value % 2 == 0;
   }
   ```

2. ```cpp
   bool isEven2(int value)
   {
      return (value & 1) == 0;
   }
   ```

Давайте обратимся к результирующему коду на ассемблере для обеих функций, для начала используем GCC 9.3 на сайте [godbolt](https://godbolt.org/):

```asm
isEven(int):
    push    rbp
    mov     rbp, rsp
    mov     DWORD PTR [rbp-4], edi
    mov     eax, DWORD PTR [rbp-4]
    and     eax, 1
    test    eax, eax
    sete    al
    pop     rbp
    ret
isEven2(int):
    push    rbp
    mov     rbp, rsp
    mov     DWORD PTR [rbp-4], edi
    mov     eax, DWORD PTR [rbp-4]
    and     eax, 1
    test    eax, eax
    sete    al
    pop     rbp
    ret
```

 Здесь мы видим, что функции абсолютно одинаковы в ассемблере, так что если говорить о GCC, то эти реализации не имеют различий. Но не одним GCC едины, так что посмотрим на код от Clang 10.0.0:

```asm
isEven(int):                             # @isEven(int)
    push    rbp
    mov     rbp, rsp
    mov     dword ptr [rbp - 4], edi
    mov     eax, dword ptr [rbp - 4]
    cdq
    mov     ecx, 2
    idiv    ecx
    cmp     edx, 0
    sete    sil
    and     sil, 1
    movzx   ecx, sil
    mov     eax, ecx
    pop     rbp
    ret
isEven2(int):                            # @isEven2(int)
    push    rbp
    mov     rbp, rsp
    mov     dword ptr [rbp - 4], edi
    mov     eax, dword ptr [rbp - 4]
    and     eax, 1
    cmp     eax, 0
    sete    cl
    and     cl, 1
    movzx   eax, cl
    pop     rbp
    ret
```



И вот здесь уже есть разница. Clang не делает оптимизацию при взятии остатка от 2, а использует `idiv` , и вот в этом эта реализация проигрывает, так как побитовое И более быстрая операция, нежели деление. Кстати, GCC также использует деление, когда вместо 2 подставляется переменная со значением 2 (ну оно и логично, как по-другому по-простому).

Если посмотреть, что там у msvc, то немного страшно смотреть на реализацию 1 функции:

```assembly
tv66 = -4                                         ; size = 4
_value$ = 8                                   ; size = 4
bool isEven(int) PROC                             ; isEven
    push    ebp
    mov     ebp, esp
    push    ecx
    mov     eax, DWORD PTR _value$[ebp]
    and     eax, -2147483647              ; 80000001H
    jns     SHORT $LN5@isEven
    dec     eax
    or      eax, -2                           ; fffffffeH
    inc     eax
$LN5@isEven:
    test    eax, eax
    jne     SHORT $LN3@isEven
    mov     DWORD PTR tv66[ebp], 1
    jmp     SHORT $LN4@isEven
$LN3@isEven:
	mov     DWORD PTR tv66[ebp], 0
$LN4@isEven:
    mov     al, BYTE PTR tv66[ebp]
    mov     esp, ebp
    pop     ebp
    ret     0
bool isEven(int) ENDP                             ; isEven

tv66 = -4                                         ; size = 4
_value$ = 8                                   ; size = 4
bool isEven2(int) PROC                              ; isEven2
    push    ebp
    mov     ebp, esp
    push    ecx
    mov     eax, DWORD PTR _value$[ebp]
    and     eax, 1
    jne     SHORT $LN3@isEven2
    mov     DWORD PTR tv66[ebp], 1
    jmp     SHORT $LN4@isEven2
$LN3@isEven2:
    mov     DWORD PTR tv66[ebp], 0
$LN4@isEven2:
    mov     al, BYTE PTR tv66[ebp]
    mov     esp, ebp
    pop     ebp
    ret     0
bool isEven2(int) ENDP                              ; isEven2
```

Но в целом, если посмотреть на количество операций, то 2 вариант выигрывает по скорости.



Таким образом, наверное, можно сказать, что 2 вариант будет работать лучше на Clang и MSVC, но все это до тех пора пока не будет включена оптимизация :)



 

## Задание 2

<details>
  <summary>Условие задания</summary>

На языках Python(2.7) и/или С++, написать минимум по 2 класса реализовывающих циклический буфер FIFO.

Объяснить плюсы и минусы каждой реализации.

</details>

С++:

1. Реализация через индексы: [ссылка](https://github.com/i582/wargaming-test-task/blob/master/cpp/src/second/cyclic_queue.h);
2. Реализация с помощью связных списков: [ссылка](https://github.com/i582/wargaming-test-task/blob/master/cpp/src/second/cyclic_queue_list.h).

У 1 варианта выигрыш в памяти, так как не нужно хранить указатель на следующий элемент связного списка. У 2 варианта выигрыш за счет более быстрого перехода после добавления/удаления на следующий элемент

Python:

1. Реализация через индексы: [ссылка](https://github.com/i582/wargaming-test-task/blob/master/python/second/main.py).



## Задание 3 (Только рассуждения)

<details>
  <summary>Условие задания</summary>

На языке Python или С/С++, написать функцию, которая быстрее всего (по процессорным тикам) отсортирует данный ей массив чисел.

Массив может быть любого размера со случайным порядком чисел (в том числе и отсортированным).

Объяснить почему вы считаете, что функция соответствует заданным критериям.

</details>



Выбрать один единственный метод из того множества сортировок, которые у нас есть невозможно. Каждый метод имеет свои плюсы и минусы. Если вспомнить первую приходящую на ум эффективную сортировку, а то есть быструю сортировку, то с ней не все так гладко она может уходить в n^2 в худшем случае. Также у нее проблема, если мы сортируем связный список, а не массив. Например, сортировке слиянием в тоже время без разницы, список это или массив. Также плюсом этой сортировки является гарантированное n\*logn время, а также возможность ее распараллеливания, например, в отличии от сортировкой кучей, которая также имеет потолок в n*logn. Но тут стоит вспомнить, что сортировка слиянием требует n памяти, в отличии от быстрой (память для рекурсии logn) и кучей (1). На маленьком количестве данных (около 20) не стоит забывать и о простых методах сортировки (вставками, выбора), так как на таком количестве n^2 не сильно отличается от n\*logn. Конечно, надо также вспомнить и о такой сортировке, как Timsort, которая является сортировкой по-умолчанию в Python. Эта сортировка ведет себя почти на уровне быстрой, но, в тоже время, на частично упорядоченных массивах показывает неплохой прирост. В С++ std::sort не имеет стандарта реализации, так в GCC это быстрая с возможным переходом в кучу. 

Из всего выше сказанного можно сделать только один вывод "Предоставьте мне ~~точку опоры~~ пример данных для сортировки и я ~~переверну весь мир~~ скажу какой алгоритм использовать" :D

