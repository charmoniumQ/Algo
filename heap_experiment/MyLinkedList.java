import java.util.Iterator;
import java.util.NoSuchElementException;

public class MyLinkedList<E> implements Iterable<E> {
	Node<E> root;
	// TODO: cache size

	public void add(E element) {
		if (root == null) {
			root = new Node<E>(element);
		} else {
			root.add(new Node<E>(element));
		}
	}

	public E remove(int idx) {
		if (root == null) {
			throw new IndexOutOfBoundsException("" + idx);
		}
		E data = root.get(idx);
		root = root.remove(idx);
		return data;
	}

	public boolean remove(E element) {
		if (root == null) {
			return false;
		}
		try {
			root = root.remove(element);
		} catch (IndexOutOfBoundsException e) {
			return false;
		}
		return true;
	}

	public boolean isEmpty() {
		return root == null;
	}

	public E get(int idx) {
		if (root == null) {
			throw new IndexOutOfBoundsException("" + idx);
		}
		return root.get(idx);
	}

	public void set(int idx, E element) {
		if (root == null) {
			throw new IndexOutOfBoundsException("" + idx);
		}
		root.set(idx, element);
	}

	public int size() {
		if (root == null) {
			return 0;
		} else {
			return root.size();
		}
	}

	public boolean contains(E element) {
		if (root == null) {
			return false;
		} else {
			return root.contains(element);
		}
	}

	public String toString() {
		if (root == null) {
			return "[]";
		} else {
			return "[" + root + "]";
		}
	}

	public Iterator<E> iterator() {
		if (root == null) {
			return new Iterator<E>(){
				public boolean hasNext() { return false; }
				public E next() { throw new NoSuchElementException(); }
				public void remove() { throw new UnsupportedOperationException(); }
			};
		} else {
			return root.iterator();
		}
	}

	private class Node<E> {
		E data;
		Node<E> next;

		Node(E element) {
			data = element;
			next = null;
		}

		void add(Node<E> element) {
			if (this.next == null) {
				this.next = element;
			} else {
				this.next.add(element);
			}
		}

		Node<E> remove(int idx) {
			if (idx == 0) {
				return this.next;
			}

			if (this.next == null) {
				throw new IndexOutOfBoundsException("" + idx);
			}
			if (idx == 1) {
				this.next = this.next.next;
				return this;
			} else {
				try {
					this.next.remove(idx - 1);
				} catch (IndexOutOfBoundsException e) {
					throw new IndexOutOfBoundsException("" + idx);
				}
				return this;
			}
		}

		Node<E> remove(E element) {
			if (this.data == element) {
				return this.next;
			}
			if (this.next == null) {
				throw new IndexOutOfBoundsException("Element " + element + " not found");
			}
			if (this.next.data == element) {
				this.next = this.next.next;
				return this;
			} else {
				this.next.remove(element);
				return this;
			}
		}

		E get(int idx) {
			if (idx == 0) {
				return this.data;
			} else {
				if (this.next == null) {
					throw new IndexOutOfBoundsException("" + idx);
				}
				return this.next.get(idx - 1);
			}
		}

		void set(int idx, E element) {
			if (idx == 0) {
				this.data = element;
			} else {
				if (this.next == null) {
					throw new IndexOutOfBoundsException("" + idx);
				}
				this.next.set(idx - 1, element);
			}
		}

		int size() {
			if (this.next == null) {
				return 1;
			} else {
				return 1 + this.next.size();
			}
		}

		boolean contains(E element) {
			if (this.data == element) {
				return true;
			} else {
				if (this.next == null) {
					return false;
				} else {
					return this.next.contains(element);
				}
			}
		}

		public String toString() {
			if (this.next == null) {
				return "" + this.data;
			} else {
				return "" + this.data + ", " + this.next;
			}
		}

		Iterator<E> iterator() {
			final Node<E> outerThis = this;

			return new Iterator<E>() {
				Node<E> current = outerThis;

				public boolean hasNext() {
					return current != null;
				}

				public E next() {
					if (current == null) {
						throw new NoSuchElementException();
					}
					Node<E> ret = current;
					current = current.next;
					return ret.data;
				}

				public void remove() {
					throw new UnsupportedOperationException();
				}
			};
		}
	}
}
