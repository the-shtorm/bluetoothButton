#ifndef MY_ENUMS_H
#define MY_ENUMS_H

enum ProjectState {
  STATE_FLASH,
  STATE_KING,
  STATE_SANJI
};


template <typename T>
class Range {
  private:
    T _min;
    T _max;
    T _current;
    bool _wrap; // True to loop around, False to hard stop at boundaries

  public:
    // Constructor
    Range(T minVal, T maxVal, T startVal, bool wrapAround = true) {
      _min = minVal;
      _max = maxVal;
      _current = startVal;
      _wrap = wrapAround;
    }

    // Update the value by a relative step (e.g., +1 or -1 from encoder)
    T change(int step) {
      T newVal = _current + step;
      
      if (_wrap) {
        if (newVal > _max) _current = _min;
        else if (newVal < _min) _current = _max;
        else _current = newVal;
      } else {
        // Clamp between min and max
        if (newVal > _max) _current = _max;
        else if (newVal < _min) _current = _min;
        else _current = newVal;
      }
      return _current;
    }

    // Getters and Setters
    T get() const { return _current; }
    void set(T val) { _current = val; }
};

#endif
