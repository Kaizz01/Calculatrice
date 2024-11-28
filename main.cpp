#include <iostream>
#include <stack>
#include <string>
#include <cctype>
#include <sstream>
#include <stdexcept>

class Calculator {
private:
    std::stack<int> operands;
    std::stack<char> operators;

    int precedence(char op) {
        if (op == '+' || op == '-') return 1;
        if (op == '*' || op == '/') return 2;
        return 0;
    }

    int applyOperation(int a, int b, char op) {
        switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/':
            if (b == 0) throw std::runtime_error("Division par zero");
            return a / b;
        default: throw std::invalid_argument("Operateur invalide");
        }
    }

    void evaluateTop() {
        if (operands.size() < 2 || operators.empty()) return;
        int b = operands.top(); operands.pop();
        int a = operands.top(); operands.pop();
        char op = operators.top(); operators.pop();
        operands.push(applyOperation(a, b, op));
    }

public:
    int evaluate(const std::string& expression) {
        for (size_t i = 0; i < expression.length(); ++i) {
            char ch = expression[i];

            if (std::isspace(ch)) continue;

            if (std::isdigit(ch)) {
                int value = 0;
                while (i < expression.length() && std::isdigit(expression[i])) {
                    value = value * 10 + (expression[i] - '0');
                    ++i;
                }
                --i;
                operands.push(value);
            }
            else if (ch == '(') {
                operators.push(ch);
            }
            else if (ch == ')') {
                while (!operators.empty() && operators.top() != '(') {
                    evaluateTop();
                }
                if (operators.empty() || operators.top() != '(') {
                    throw std::invalid_argument("Parenthese fermante sans correspondance");
                }
                operators.pop();
            }
            else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
                while (!operators.empty() && precedence(operators.top()) >= precedence(ch)) {
                    evaluateTop();
                }
                operators.push(ch);
            }
        }

        while (!operators.empty()) {
            if (operators.top() == '(') {
                throw std::invalid_argument("Parenthese ouvrante sans correspondance");
            }
            evaluateTop();
        }

        return operands.top();
    }
};

int main() {
    Calculator calc;
    std::string expression;

    std::cout << "Entrez une expression arithmetique : ";
    std::getline(std::cin, expression);

    try {
        int result = calc.evaluate(expression);
        std::cout << "Resultat : " << result << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Erreur : " << e.what() << std::endl;
    }

    return 0;
}