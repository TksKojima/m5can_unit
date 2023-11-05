
#ifndef MY_FIFO_H
#define MY_FIFO_H


// #include <iostream>
// #include <vector>

template <typename T>
class FIFO {
private:
    std::vector<T> data;

public:
    // FIFOに要素を追加
    void push(T item) {
        data.push_back(item);
    }

    // FIFOから要素を取り出す
    T pop() {
        if (data.empty()) {
            throw std::out_of_range("FIFO is empty");
        }
        T item = data.front();
        data.erase(data.begin());
        return item;
    }

    // FIFOが空かどうかを確認
    bool isEmpty() const {
        return data.empty();
    }

    // FIFOの要素数を取得
    size_t size() const {
        return data.size();
    }

    // FIFOの先頭の要素を取得
    T front() const {
        if (data.empty()) {
            throw std::out_of_range("FIFO is empty");
        }
        return data.front();
    }
};

// // 使用例
// int main() {
//     FIFO<int> fifo;
//     fifo.push(5);
//     fifo.push(10);
//     fifo.push(15);

//     std::cout << "Front element: " << fifo.front() << std::endl;
//     fifo.pop();
//     std::cout << "Front element after pop: " << fifo.front() << std::endl;

//     std::cout << "FIFO size: " << fifo.size() << std::endl;

//     return 0;
// }


#endif