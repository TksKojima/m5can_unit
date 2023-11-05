
#ifndef MY_FIFO_H
#define MY_FIFO_H


// #include <iostream>
// #include <vector>

template <typename T>
class FIFO {
private:
    std::vector<T> data;

public:
    // FIFO�ɗv�f��ǉ�
    void push(T item) {
        data.push_back(item);
    }

    // FIFO����v�f�����o��
    T pop() {
        if (data.empty()) {
            throw std::out_of_range("FIFO is empty");
        }
        T item = data.front();
        data.erase(data.begin());
        return item;
    }

    // FIFO���󂩂ǂ������m�F
    bool isEmpty() const {
        return data.empty();
    }

    // FIFO�̗v�f�����擾
    size_t size() const {
        return data.size();
    }

    // FIFO�̐擪�̗v�f���擾
    T front() const {
        if (data.empty()) {
            throw std::out_of_range("FIFO is empty");
        }
        return data.front();
    }
};

// // �g�p��
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