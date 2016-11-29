public class MyPQ implements PriorityQueue {
	private static final double phi = (1.0d + Math.sqrt(5.0d)) / 2.0d;

	MyLinkedList2<FibonacciNode> roots;
	int size;
	public long t1, t2, t3;

	public MyPQ() {
		makeEmpty();
	}

	public boolean isEmpty() {
		return size == 0;
	}

	public int size() {
		return size;
	}

	public double findMin() {
		if (roots.isEmpty()) {
			throw new EmptyPQException("Cannot find min from an empty heap");
		}
		return roots.peekFirst().data;
	}

	public void insert(double element) {
		size++;
		FibonacciNode newNode = new FibonacciNode(element);
		if (roots.isEmpty()) {
			roots.addFirst(newNode);
		} else {
			if (element < findMin()) {
				roots.addFirst(newNode);
			} else {
				roots.addSecond(newNode);
			}
		}
		assert check(): "Faulty internal state";
	}

	public double deleteMin() {
		if (roots.isEmpty()) {
			throw new EmptyPQException("Cannot delete min from an empty heap");
		}
		long start, stop;

		start = System.nanoTime();
		FibonacciNode min = mameMin();
		stop = System.nanoTime();
		t1 += (stop - start) / 1000;
		assert check(false): "Faulty internal state";

		start = System.nanoTime();
		compress();
		stop = System.nanoTime();
		t2 += (stop - start) / 1000;
		assert check(false): "Faulty internal state";

		start = System.nanoTime();
		roots.minFront();
		stop = System.nanoTime();
		t3 += (stop - start) / 1000;

		assert check(): "Faulty internal state";
		return min.data;
	}

	private FibonacciNode mameMin() {
		size--;
		FibonacciNode min = roots.popFirst();
		roots.addAll(min.children);
		min.children.clear();
		return min;
	}

	private void compress() {
		// combine trees of the same degree by making the one with a larger root the child of the one with the smaller root until every tree has a different degree
		if (roots.size() <= 1) {
			return;
		}

		// TODO: make newRoots smaller
		FibonacciNode[] newRoots = combineDegree((int) Math.ceil(Math.log(size + 2) / Math.log(phi)));
		rootsArray(newRoots);
	}

	private void rootsArray(FibonacciNode[] newRoots) {
		roots = new MyLinkedList2<FibonacciNode>();
		for (int i = 0; i < newRoots.length; ++i) {
			if (newRoots[i] != null) {
				roots.addFirst(newRoots[i]);
			}
		}
	}

	private FibonacciNode[] combineDegree(int maxdegree) {
		FibonacciNode[] newRoots = new FibonacciNode[maxdegree];
		MyLinkedList2<FibonacciNode>.MIterator<FibonacciNode> it = roots.iterator();
		FibonacciNode current = it.next();
		while (true) {
			int degree = current.degree();
			if (newRoots[degree] == null) {
				newRoots[degree] = current;
				if (! it.hasNext()) {
					break;
				}
				current = it.next();
			} else {
				if (current.data > newRoots[degree].data) {
					attach(newRoots[degree], current);
					current = newRoots[degree];
				} else {
					attach(current, newRoots[degree]);
				}
				newRoots[degree] = null;
			}
		}
		return newRoots;
	}

	public void makeEmpty() {
		roots = new MyLinkedList2<FibonacciNode>();
		size = 0;
		assert check(): "Faulty internal state";
	}

	protected boolean check() { return check(true); }

	protected boolean check(boolean checkmin) {
		if (checkmin) {
			if (! roots.isEmpty()) {
				for (FibonacciNode root : roots) {
					if (root.data < roots.peekFirst().data) {
						System.out.println("Min is not at the front of the list");
						return false;
					}
				}
			}
		}
		for (FibonacciNode root : roots) {
			if (root == null) {
				System.out.println("null root");
				System.out.println(roots);
				return false;
			}
			if (!root.check()) {
				System.out.println("root does not have min-heap property");
				System.out.println(root);
				System.out.println(toStringDebug());
				return false;
			}
		}
		int sum = 0;
		for (FibonacciNode root : roots) {
			sum += root.size();
		}
		if (size != sum) {
			sum = 0;
			for (FibonacciNode root : roots) {
				sum += root.size();
				System.out.println(root + ": " + root.size() + " =+ " + sum + " / " + size);
			}
			System.out.println("bad size");
			System.out.println("sum = " + sum + ", size = " + size);
			System.out.println(toStringDebug());
			return false;
		}
		return true;
	}

	static void attach(FibonacciNode parent, FibonacciNode child) {
		parent.children.addFirst(child);
	}

	private String toStringDebug() {
		StringBuilder sb = new StringBuilder("[");
		for (FibonacciNode root : roots) {
			sb.append(root.toStringDebug());
			sb.append(", ");
		}
		if (! roots.isEmpty()) {
			sb.delete(sb.length() - 2, sb.length());
		}
		sb.append("]");
		return sb.toString();
	}

	public String toString() {
		StringBuilder sb = new StringBuilder("[");
		for (FibonacciNode root : roots) {
			sb.append(root.toString());
			sb.append(", ");
		}
		if (! roots.isEmpty()) {
			sb.delete(sb.length() - 2, sb.length());
		}
		sb.append("]");
		return sb.toString();
	}

	class FibonacciNode implements Comparable<FibonacciNode> {
		double data;
		boolean marked;
		MyLinkedList2<FibonacciNode> children;
		int size;

		FibonacciNode(double element) {
			data = element;
			marked = false;
			children = new MyLinkedList2<FibonacciNode>();
		}

		boolean check() {
			// TODO: check the number of children against the degree
			for (FibonacciNode child : children) {
				if (! child.check()) {
					return false;
				}
			}
			return true;
		}

		int size() {
			// TODO: cache size
			int sum = 1;
			for (FibonacciNode child : children) {
				sum += child.size();
			}
			return sum;
		}

		int degree() {
			return children.size();
		}

		public String toString() {
			if (degree() == 0) {
				return String.format("%.2f", data);
			} else {
				return String.format("%.2f %s", data, children.toString());
			}
		}

		String toStringDebug() {
			if (degree() == 0) {
				return String.format("%.2f", data);
			} else {
				StringBuilder sb =  new StringBuilder();
				sb.append(String.format("%.2f [", data));
				for (FibonacciNode child : children) {
					sb.append(child.toStringDebug());
					sb.append(", ");
				}
				sb.delete(sb.length() - 2, sb.length());
				sb.append("]");
				return sb.toString();
			}
		}

		public int compareTo(FibonacciNode other) {
			if (this.data > other.data) {
				return 1;
			}
			if (this.data < other.data) {
				return -1;
			}
			return 0;
		}
	}
}
