import java.util.Collections;
import java.util.ArrayList;

public class TestHeap {
	public static void main(String args[]) {
		MyPQ test = new MyPQ();

		testEmpty(test);
		testInsert(test);
		testMin(test);
		testSize(test);

		System.out.println("Done testing");
	}


	private static void testEmpty(PriorityQueue test) {
		for (int i = 0; i < NUM_TESTS; ++i) {
			// insert a random number of elements
			ArrayList<Double> data = randomData();
			for (double d : data) {
				test.insert(d);
			}
			myAssert(! test.isEmpty(), "Incorrect empty (false positive) after insertion");
			test.makeEmpty();
			myAssert(test.isEmpty(), "Incorrect empty (false negative) after make empty");
			data = randomData();
			int size = data.size();

			for (double d : data) {
				test.insert(d);
				if (Math.random() > 0.7) {
					test.deleteMin();
					size--;
				}
			}
			for (int j = 0; j < size; ++j) {
				myAssert(! test.isEmpty(), "Incorrect empty (false positive) after insert/delete");
				test.deleteMin();
			}
			myAssert(test.isEmpty(), "Incorrect empty (false negative) after delete");
		}
	}

	private static void testInsert(PriorityQueue test) {
		test.makeEmpty();
		for (int i = 0; i < NUM_TESTS; ++i) {
			// intialize random queue
			ArrayList<Double> data = randomData();
			for (double d : data) {
				test.insert(d);
			}

			while (! data.isEmpty()) {
				myAssert(! test.isEmpty(), "test missed an alement");
				if (test.isEmpty()) {
					break;
				}
				double d = test.deleteMin();
				myAssert(data.contains(d), "non-existent element came out of queue");
				data.remove(d);

				// but sometimes add an element (1 / 10 chance)
				if (Math.random() > 0.9) {
					double j = randomDouble();
					data.add(j);
					test.insert(j);
				}
			}
		}
	}

	private static String toString(ArrayList<Double> l) {
		StringBuilder b = new StringBuilder("[");
		for (double d : l) {
			b.append(String.format("%.2f, ", d));
		}
		if (l.size() > 0) {
			b.deleteCharAt(b.length() - 1);
			b.deleteCharAt(b.length() - 1);
		}
		b.append("]");
		return b.toString();
	}

	private static void testMin(PriorityQueue test) {
		for (int i = 0; i < NUM_TESTS; ++i) {
			test.makeEmpty();
			// intialize random queue
			ArrayList<Double> data = randomData();
			for (double d : data) {
				test.insert(d);
			}

			// check the minimum is correct ten times or until out of data
			for (int j = 0; j < 10 && data.size() != 0; ++j) {
				Collections.sort(data);

				myAssert(test.findMin() == data.get(0), "Incorrect find minimum");
				myAssert(test.deleteMin() == data.get(0), "Incorrect delete minimum");
				data.remove(0);

				// but sometimes add an element
				if (Math.random() > 0.6) {
					double d = randomDouble();
					data.add(d);
					test.insert(d);
				}

				// and sometimes reset the list
				if (Math.random() > 0.9) {
					ArrayList<Double> data2 = randomData();
					data.clear();
					test.makeEmpty();
					for (double d : data2) {
						data.add(d);
						test.insert(d);
					}
				}
			}
		}
	}

	private static void testSize(PriorityQueue test) {
		test.makeEmpty();
		ArrayList<Double> data = randomData();
		for (double d : data) {
			test.insert(d);
		}
		for (int i = 0; i < NUM_TESTS; ++i) {
			// assert size is the same as the data behind it
			myAssert(data.size() == test.size(), "Incorrect size");

			// but sometimes replace the whole queue
			if (Math.random() > 0.7) {
				test.makeEmpty();
				data.clear();
				ArrayList<Double> data2 = randomData();
				for (double d : data2) {
					data.add(d);
					test.insert(d);
				}
			}

			// and sometimes insert an element
			if (Math.random() > 0.6) {
				double d = randomDouble();
				test.insert(d);
				data.add(d);
			}

			// and sometimes remove an element
			if (Math.random() > 0.6) {
				if (! data.isEmpty()) {
					double d = test.deleteMin();
					data.remove(d);
				}
			}
		}
	}

	private static final int maxDouble = 100;
	private static final int maxCount = 10;
	private static final int NUM_TESTS = 50;

	private static double randomDouble() {
		return Math.random() * 2 * maxDouble - maxDouble;
	}

	private static ArrayList<Double> randomData() {
		return randomData((int) (Math.random() * maxCount + 1));
	}

	private static ArrayList<Double> randomData(int count) {
		ArrayList<Double> data = new ArrayList<Double>();
		for (int i = 0; i < count; ++i) {
			data.add(randomDouble());
		}
		return data;
	}

	private static void myAssert(boolean cond, String message) {
		if (! cond) {
			System.err.println(message);
		}
	}
}
