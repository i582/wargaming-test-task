class CyclesQueue:
    def __init__(self, size):
        self._items = [0] * size
        self._head = 0
        self._tail = 0

        self._count = 0
        self._size = size

    def push(self, el):
        if self.full():
            raise Exception("Queue is full!")

        self._items[self._head] = el

        self._count += 1
        self._head = self.next_index(self._head)

    def pop(self):
        if self.empty():
            raise Exception("Queue is empty!")

        item = self._items[self._tail]

        self._count -= 1

        self._tail = self.next_index(self._tail)
        return item

    def count(self):
        return self._count

    def size(self):
        return self._size

    def full(self):
        return self._count == self._size

    def empty(self):
        return self._count == 0

    def next_index(self, index):
        temp = index + 1
        if temp >= self._size:
            temp = 0
        return temp


queue = CyclesQueue(2)

queue.push(100)
queue.push(100)

queue.pop()

queue.push(100)
