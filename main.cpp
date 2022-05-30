#include <iostream>
#include <iomanip>

#include <cstdlib>

using namespace std;

enum ERRORS {
    NO_ERROR = 0,
    INVALID_PTR = -1,
    INVALID_ARGUMENTS = -2
};

struct Matrix {
    size_t iRows = 0;
    size_t iColumns = 0;
    // [строки][столбцы]
    int **arrMatrix = nullptr;

    ~Matrix() {
        for (size_t iRow = 0; iRow < iRows; ++iRow) {
            delete[] arrMatrix[iRow];
        }

        delete[] arrMatrix;
    }
};

int GetValue();

Matrix *Create(int iRows, int iColumns, int *iError = nullptr);

int Print(Matrix *pMatrix);

int Fill(Matrix *pMatrix);

int ShiftRight(Matrix *pMatrix, size_t iShift);

int ShiftDown(Matrix *pMatrix, size_t iShift);

void TaskSolution();

int main() {
    TaskSolution();

    return 0;
}

void TaskSolution() {
    setlocale(LC_ALL, "Russian");

    int iRows, iColumns;
    size_t iShift = 0;
    Matrix *pMatrix = nullptr;

    int iError;
    while (true) {
        int iMenu = 0;
        cout << "Введите пункт меню:\n";
        cout << "1. Создать матрицу\n";
        cout << "2. Заполнить матрицу\n";
        cout << "3. Сдвинуть матрицу вправо\n";
        cout << "4. Сдвинуть матрицу вниз\n";
        cout << "5. Вывод матрицы в консоль\n";
        cout << "Любая другая цифра - Выход" << endl;

        iMenu = GetValue();

        switch (iMenu) {
            case 1:
                if (pMatrix != nullptr) {
                    delete pMatrix;
                }

                pMatrix = nullptr;

                cout << "Пожалуйста, введите количество строк матрицы" << endl;
                iRows = GetValue();
                cout << "Пожалуйста, введите количество столбцов матрицы" << endl;
                iColumns = GetValue();

                pMatrix = Create(iRows, iColumns, &iError);

                break;
            case 2:
                iError = Fill(pMatrix);

                break;
            case 3:
                cout
                        << "Пожалуйста, введите на сколько позиций сдвинуть матрицу вправо. Значение должно быть положительным"
                        << endl;
                iShift = static_cast<size_t>(GetValue());

                iError = ShiftRight(pMatrix, iShift);

                break;
            case 4:
                cout
                        << "Пожалуйста, введите на сколько позиций сдвинуть матрицу вниз. Значение должно быть положительным"
                        << endl;
                iShift = static_cast<size_t>(GetValue());

                iError = ShiftDown(pMatrix, iShift);

                break;
            case 5:
                iError = Print(pMatrix);

                break;
            default:
                delete pMatrix;

                return;
        }

        if (iError == INVALID_PTR) {
            cout << "Ошибка: матрица не создана";
        } else if (iError == INVALID_ARGUMENTS) {
            cout << "Ошибка: проверьте вводимые параметры";
        }

        cout << '\n';
        cout << endl;
    }
}

int GetValue() {
    bool is_first = true;
    int value = 0;

    do {
        if (is_first) {
            is_first = false;
        } else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Неверный ввод. Значение должно быть действительным.\n";
        }
        cin >> value;
    } while (!cin.good());

    return value;
}

Matrix *Create(int iRows, int iColumns, int *iError) {
    if (iRows < 1 || iColumns < 1) {
        if (iError != nullptr) {
            *iError = INVALID_ARGUMENTS;
        }

        return nullptr;
    }

    auto *pMatrix = new Matrix();

    pMatrix->iRows = iRows;

    pMatrix->iColumns = iColumns;

    pMatrix->arrMatrix = new int *[iRows];

    for (size_t i = 0; i < iRows; ++i) {
        pMatrix->arrMatrix[i] = new int[iColumns];

        memset(pMatrix->arrMatrix[i], 0, sizeof(iColumns));
    }

    if (iError != nullptr) {
        *iError = NO_ERROR;
    }

    return pMatrix;
}

int Fill(Matrix *pMatrix) {
    if (pMatrix == nullptr) {
        return INVALID_PTR;
    }

    for (size_t i = 0; i < pMatrix->iRows; i++) {
        for (size_t j = 0; j < pMatrix->iColumns; j++) {
            cout << "Введите элемент [" << i + 1 << "][" << j + 1 << "] ";
            pMatrix->arrMatrix[i][j] = GetValue();
        }
    }

    return NO_ERROR;
}

int Print(Matrix *pMatrix) {
    if (pMatrix == nullptr) {
        return INVALID_PTR;
    }

    for (size_t iRow = 0; iRow < pMatrix->iRows; ++iRow) {
        for (size_t iColumn = 0; iColumn < pMatrix->iColumns; ++iColumn) {
            cout << setw(11) << pMatrix->arrMatrix[iRow][iColumn];
        }
        cout << endl;
    }

    return NO_ERROR;
}

int ShiftRight(Matrix *pMatrix, size_t iShift) {
    if (pMatrix == nullptr) {
        return INVALID_PTR;
    }

    if (iShift == 0) {
        return INVALID_ARGUMENTS;
    }

    for (size_t iRow = 0; iRow < pMatrix->iRows; ++iRow) {
        int *arrTempColumn = new int[pMatrix->iColumns];

        for (size_t iColumn = 0; iColumn < pMatrix->iColumns; ++iColumn) {
            arrTempColumn[iColumn] = pMatrix->arrMatrix[iRow][iColumn];
        }

        for (size_t iColumn = 0; iColumn < pMatrix->iColumns; ++iColumn) {
            size_t iCurrentShift = (iColumn + iShift) % pMatrix->iColumns;

            pMatrix->arrMatrix[iRow][iCurrentShift] = arrTempColumn[iColumn];
        }

        delete[] arrTempColumn;
    }

    return NO_ERROR;
}

int ShiftDown(Matrix *pMatrix, size_t iShift) {
    if (pMatrix == nullptr) {
        return INVALID_PTR;
    }

    if (iShift == 0) {
        return INVALID_ARGUMENTS;
    }

    int **arrRows = new int *[pMatrix->iRows];
    memcpy(arrRows, pMatrix, pMatrix->iRows);

    for (size_t iRow = 0; iRow < pMatrix->iRows; ++iRow) {
        arrRows[iRow] = pMatrix->arrMatrix[iRow];
    }


    for (size_t iRow = 0; iRow < pMatrix->iRows; ++iRow) {
        size_t iCurrentShift = (iRow + iShift) % pMatrix->iRows;

        pMatrix->arrMatrix[iCurrentShift] = arrRows[iRow];
    }

    delete[] arrRows;

    return NO_ERROR;
}
