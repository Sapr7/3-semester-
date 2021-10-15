#include <iostream>
#include <map>

class Grid {
private:
    float *memory;
    size_t _x_size, _y_size;
public:
    Grid(size_t x_size, size_t y_size) {
        _x_size = x_size;
        _y_size = y_size;
        memory = new float[_x_size * _y_size];
    }
    ~Grid() {
        delete[] memory;
    }
    float operator()(size_t x_idx, size_t y_idx) const {
        if (x_idx >= _x_size || y_idx >= _y_size) throw std::exception();
        return memory[y_idx * _x_size + x_idx];
    }
    float &operator()(size_t x_idx, size_t y_idx) {
        if (x_idx >= _x_size || y_idx >= _y_size) throw std::exception();
        return memory[y_idx * _x_size + x_idx];
    }
    size_t get_xsize() const {
        return _x_size;
    }
    size_t get_ysize() const {
        return _y_size;
    }
    size_t get_size() const {
        return _x_size * _y_size;
    }
    Grid &operator=(float val) {
        for (size_t i = 0; i < _y_size * _x_size; i++) {
            memory[i] = val;
        }
        return *this;
    }
    Grid &operator=(const Grid &drob) {
        if (&drob == this)return *this;
        size_t S = drob.get_size();
        if (_x_size * _y_size != S) {
            delete[] memory;
            memory = new float[S];
        }
        _x_size = drob.get_xsize();
        _y_size = drob.get_ysize();
        for (size_t i = 0; i < S; i++) {
            memory[i] = drob.memory[i];
        }
        return *this;
    }

    friend std::ostream &operator<<(std::ostream &out, const Grid &grid) {
        size_t Y = grid.get_ysize(), X = grid.get_xsize();
        for (size_t y = 0, i = 0; y < Y; y++) {
            for (size_t x = 0; x < X; x++, i++) {
                out << (grid(x, y)) << ' ';
            }
            out << '\n';
        }
        return out;
    }
    friend std::istream &operator>>(std::istream &in, Grid &grid) {
        size_t Y = grid.get_ysize(), X = grid.get_xsize();
        for (size_t y = 0, i = 0; y < Y; y++) {
            for (size_t x = 0; x < X; x++, i++) {
                in >> (grid(x, y));
            }
        }
        return in;
    }
};

int main() {

    return 0;
}
