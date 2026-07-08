18. Design Pizza Pricing System
Asked in
Amazon LLD
Implement low level design of pizza pricing system.
System should support initializing a new pizza, adding toppings to it and calculating final price of pizza.
Toppings Catalog (With Per-Serving Cost in units)
Topping	Cost per serving
cheeseburst	100
corn	50
onion	30
capsicum	50
pineapple	60
mushroom	40
Business Rules that your system must follow:
Cheeseburst Tax Uplift
Cheeseburst is considered unhealthy food and attracts more tax.
If at least one serving of cheeseburst is present, increase the pizza’s entire tax rate by +30% of its current value (one-time uplift).
Example: base tax 10 → 10 + 0.30 × 10 = 13.
Additional cheeseburst servings do not further change tax.
Cheeseburst Volume Discount
First serving costs 100; each additional serving costs 70.
Health Constraint (Mutual Exclusion)
mushroom is considered healthy food and it cannot be added if cheeseburst is already present, also cheeseburst cannot be added if mushroom is already present.
On violation, addTopping returns false and there is no state change.
Your implementation should make it easy to add new rules later (for example, size-based caps, combos, promos) .
Use decorator design pattern .

Constructor and Methods
PizzaPricing(int basePrice, int taxPercentage, String size)
		
100 ≤ basePrice ≤ 10000, and basePrice will always be a multiple of 100.

0 ≤ taxPercentage ≤ 1000 1000% tax means tax is 10 times the base price of pizza.
i.e. if base price is 200 units and tax 1000 and no toppings are there,
then final price = 200+ tax = 200* 1000% of 200 = 200 + 1000/100 * 200 = 2,200/- units
Another example: basePrice = 200 units, taxPercentage = 18% then,
Final price = 200 + 18/100 * 200 =200+36 = 236 units.

size is one of small, medium, large.

All constructor inputs are guaranteed to be valid.
Methods
boolean addTopping(String topping, int servingsCount)
servingsCount is a valid positive integer.
Returns true if the topping is applied (passes all rules); otherwise false and no state change.
int getFinalPrice()
Returns the final price rounded to int using Round Half Up (see rounding).
Can be called any time.
Pricing and Tax Model
Subtotal = basePrice + Σ(topping costs actually applied)
Taxable Base = Subtotal
Tax Rate starts at the constructor-supplied taxPercentage. Rules can make it fractional (for example, +30% of current tax).
Tax Amount = (Tax Rate / 100) × Subtotal
Final Price (pre-round) = Subtotal + Tax Amount
Final price Rounding Rule
To convert final price to int: (int)(x + 0.5).

4.6 → 5; 4.51 → 5; 4.49 → 4; 4.5 → 5
Worked Examples
Example A - Uses all methods; cheeseburst uplift + other toppings
 Create pizza
	PizzaPricing p = new PizzaPricing(200, 10, "small");

	 Add toppings
	p.addTopping("cheeseburst", 1);   // true, tax 10 → 13 (one-time uplift)
	p.addTopping("onion", 2);         // true

	Compute final price
	int price = p.getFinalPrice();    // 407

	 Calculation: base=200; toppings=100 + 2×30=160; subtotal=360;
	 tax=13% of 360=46.8; final=406.8 → round half up ⇒ 407
	 
Example B - Mutual exclusion blocks cheeseburst
   Create pizza
	PizzaPricing q = new PizzaPricing(350, 8, "medium");

	 Add toppings
	q.addTopping("mushroom", 2);      // true (2×40=80)
	q.addTopping("cheeseburst", 1);   // false (blocked by mutual exclusion)

	 Compute final price
	int price = q.getFinalPrice();    // 464

	 Calculation: base=350; toppings=80; subtotal=430;
	 tax=8% of 430=34.4; final=464.4 → round half up ⇒ 464
	 