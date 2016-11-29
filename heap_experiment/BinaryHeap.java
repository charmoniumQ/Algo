public class BinaryHeap implements PriorityQueue {
	public BinaryHeap() {
		makeEmpty();
	}

	public void makeEmpty() {
		makeEmpty(DEFAULT_SIZE);
	}

	void setSize(int newSize) {
		if (newSize >= data.length - 1) {
			allocate((int) (Math.max(newSize, 2) * 1.5));
		}
		size = newSize;
	}

	public int size() {
		return size;
	}

	void allocate(int newSize) {
		if (newSize < size) {
			throw new IndexOutOfBoundsException(String.format("Cannot allocate %d elements into an array of %d", size, newSize));
		}
		double[] oldData = data;
		data = new double[newSize + 1];
		for (int i = 0; i < size; ++i) {
			data[i] = oldData[i];
		}
		assert check(): "bad check";
	}

	public void trimToSize() {
		allocate(size);
	}

	int allocated() {
		return data.length - 1;
	}

	void makeEmpty(int size) {
		data = new double[size + 1];
		setSize(0);
		assert check(): "bad check";
	}

	int[] children(int s) {
		if (s * 2 + 1 <= size) {
			return new int[]{s*2, s*2 + 1};
		} else if (s * 2 <= size) {
			return new int[]{s*2};
		} else {
			return new int[]{};
		}
	}

	int parent(int s) {
		if (s == 1) {
			return 0;
		} else {
			return s / 2;
		}
	}

	public boolean isEmpty() {
		return size == 0;
	}

	public double findMin() {
		if (isEmpty()) {
			throw new EmptyPQException("Unable to find min from an empty heap");
		} else {
			return data[1];
		}
	}

	public void insert(double x) {
		setSize(size + 1);
		data[size] = x;
		upheapify(size);
		assert check(): "bad check";
	}

	public double deleteMin() {
		double min = findMin();
		if (! isEmpty()) {
			data[1] = data[size];
			data[size] = 0; // comment this out for lazy deletion
			size--;
			downheapify(1);
		}
		assert check(): "bad check";
		return min;
	}

	void upheapify(int s) {
		if (s <= 1) {
			return;
		}
		if (data[s] < data[parent(s)]) {
			swap(s, parent(s));
			upheapify(parent(s));
		}
	}

	void downheapify(int s) {
		int[] myChildren = children(s);
		if (myChildren.length == 0) {
			// leaf node, done downheapify
			return;
		}
		int smallestChild = myChildren[0];
		double smallestValue = data[myChildren[0]];
		for (int i = 1; i < myChildren.length; ++i) {
			if (data[myChildren[i]] < smallestValue) {
				smallestValue = data[myChildren[i]];
				smallestChild = myChildren[i];
			}
		}
		if (data[s] > smallestValue) {
			swap(s, smallestChild);
			downheapify(smallestChild);
		}
	}

	void swap(int a, int b) {
		double temp = data[a];
		data[a] = data[b];
		data[b] = temp;
	}

	String toString(int i) {
		if (i > size) {
			return "";
		}
		StringBuilder b = new StringBuilder();
		b.append(String.format("%.2f, ", data[i]));
		boolean first = true;
		for (int child : children(i)) {
			if (first) {
				b.append("[");
				first = false;
			}
			b.append(toString(child) + ", ");
		}
		if (first) {
			// No children
			// Delete the last char (being ', ')
			b.delete(b.length()-2, b.length());
		} else {
			// Has children
			// Delete the last char (being ', ')
			b.deleteCharAt(b.length()-1);
			b.deleteCharAt(b.length()-1);
			// add a ]
			b.append("]");
		}
		return b.toString();
	}

	public String toString() {
		if (size == 0) {
			return "[]";
		} else {
			return "[" + toString(1) + "]";
		}
	}

	protected boolean check() {
		if (size == 0) {
			return true;
		}
		if (size >= data.length) {
			return false;
		}
		int[] queue = new int[size];
		queue[0] = 1;
		int queueSize = 1;
		while (queueSize > 0) {
			int current = queue[queueSize - 1];
			queueSize--;
			for (int child : children(current)) {
				if (data[current] > data[child]) {
					return false;
				}
				queue[queueSize] = child;
				queueSize++;
			}
		}
		return true;
	}

	private static final int DEFAULT_SIZE = 16;
	int size;
	double[] data;
}
