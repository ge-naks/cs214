def calculate_tax(brackets, income):
    total_tax = 0
    remaining_income = income

    for i, (cutoff, rate) in enumerate(brackets):
        next_cutoff = brackets[i+1][0] if i < len(brackets) - 1 else income
        taxable_amount = min(remaining_income, next_cutoff - cutoff)
        total_tax += taxable_amount * rate
        remaining_income -= taxable_amount

        if remaining_income <= 0:
            break

    return total_tax

def main():
    brackets = []
    n = int(input("Enter the number of tax brackets: "))

    print("Enter the cutoffs and rates for each bracket:")
    for _ in range(n):
        cutoff, rate = map(float, input().split())
        brackets.append((cutoff, rate))

    income = float(input("Enter the income: "))

    tax = calculate_tax(brackets, income)
    print(f"The total tax owed is: {tax:.2f}")

if __name__ == "__main__":
    main()