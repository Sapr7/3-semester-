#include <iostream>
#include <map>
#include <set>

class Grid {
private:
    float *memory;
    size_t _x_size, _y_size;
    std::map<size_t, Grid> subgrids = {};
public:
    Grid(size_t x_size, size_t y_size) {
        _x_size = x_size;
        _y_size = y_size;
        memory = new float[_x_size * _y_size];
    }
    Grid() : Grid(0, 0) { }
    Grid(const Grid &other) : Grid() {
        *this = other;
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
    double getAvg() const {
        size_t S = get_size();
        double sum = 0;
        for (size_t i = 0; i < S; i++) {
            sum += memory[i];
        }
        return sum / (double) S;
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
                if (grid.is_subgrid(x, y)) {
                    out << '#';
                } else {
                    out << (grid(x, y)) << ' ';
                }
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

    void make_subgrid(size_t x_idx, size_t y_idx, size_t x_sub_size, size_t y_sub_size) {
        Grid subgrid(x_sub_size, y_sub_size);
        size_t i = x_idx + y_idx * _y_size;
        subgrids.insert({i, subgrid});
    }

    Grid &get_subgrid(size_t i) {
        return subgrids[i];
    }
    const Grid &get_subgrid(size_t i) const {
        return subgrids.at(i);
    }
    Grid &get_subgrid(size_t x_idx, size_t y_idx) {
        return subgrids[x_idx + y_idx * _y_size];
    }
    const Grid &get_subgrid(size_t x_idx, size_t y_idx) const {
        return subgrids.at(x_idx + y_idx * _y_size);
    }
    // этому методу не нужны параметры кроме индексов
    Grid &collapse_subgrid(size_t x_idx, size_t y_idx) {
        size_t i = x_idx + y_idx * _y_size;
        Grid &subgrid = get_subgrid(i);
        double m = subgrid.getAvg();
        memory[i] = (float) m;
        subgrids.erase(i);
        return subgrid;
    }

    bool is_subgrid(size_t x_idx, size_t y_idx) const {
        size_t i = x_idx + y_idx * _y_size;
        return subgrids.find(i) != subgrids.end();
    }
};

int main() {
    Grid G0(3, 3), G1;
    std::cin >> G0;
    G0(2, 1) = -10;
    G1 = G0;
    G1(0, 0) = 5.5;
    std::cout << G0 << G1;
    G1.make_subgrid(1, 1, 2, 2);
    G1.get_subgrid(1, 1)(1, 1) = 9;
    std::cout << G1;
    return 0;
}
