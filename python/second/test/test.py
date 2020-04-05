import unittest
from main import *


class TestCyclesQueueMethods(unittest.TestCase):

    def test(self):
        queue1 = CyclesQueue(3)

        queue1.push(1)
        queue1.push(2)
        queue1.push(3)

        try:
            queue1.push(100)
        except Exception:
            self.assertTrue(True)

        self.assertEqual(queue1.pop(), 1)
        self.assertEqual(queue1.pop(), 2)
        self.assertEqual(queue1.pop(), 3)

        try:
            queue1.pop()
        except Exception:
            self.assertTrue(True)

        queue1.push(1)
        queue1.push(2)
        queue1.pop()
        queue1.push(3)

        queue1.pop()
        queue1.pop()
        queue1.push(3)
        queue1.push(4)

        self.assertEqual(queue1.pop(), 3)
        self.assertEqual(queue1.pop(), 4)


if __name__ == '__main__':
    unittest.main()
