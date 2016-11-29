import java.util.Iterator;
import java.util.NoSuchElementException;

public class MyLinkedList2<E extends Comparable<E>> implements Iterable<E> {
	Node<E> root;
        Node<E> tail;
	int size = 0;
        
        MyLinkedList2() {
            clear();
        }

        public void addAll(MyLinkedList2<E> elements) {
            if (elements.size == 0) {
                return;
            }
            if (size == 0) {
                root = elements.root;
                size = elements.size;
                tail = elements.tail;
            } else {
                tail.next = elements.root;
                tail = elements.tail;
                size += elements.size;
            }
        }

        public void clear() {
            root = null;
            tail = null;
            size = 0;
        }
	public void minFront() {
		if (root == null || root.next == null) {
			// only zero or one node in list; min is at front
			return;
		}
		Node<E> current = root.next, previous = root;
		while (current != null) {
			if (current.data.compareTo(root.data) < 0) {
				E tmp = root.data;
				root.data = current.data;
				current.data = tmp;
			}
			previous = current;
			current = current.next;
		}
                tail = previous;
	}

	public void addFirst(E element) {
		root = new Node<E>(element, root);
                if (root.next == null) {
                    tail = root;
                }
		size++;
	}

	public void addSecond(E element) {
		root.next = new Node<E>(element, root.next);
                if (root.next.next == null) {
                    tail = root.next;
                }
		size++;
	}

	public E popFirst() {
		if (root == null) {
			throw new IndexOutOfBoundsException("Cannot pop from an empty list");
		}
		E element = root.data;
		root = root.next;
		size--;
                if (root == null) {
                    tail = null;
                }
		return element;
	}

	public boolean isEmpty() {
		return root == null;
	}

	public int size() {
		return size;
	}

	public E peekFirst() {
		if (root == null) {
			throw new IndexOutOfBoundsException("Cannot peek from an empty list");
		}
		return root.data;
	}

	public String toString() {
		if (root == null) {
			return "[]";
		} else {
			StringBuilder sb = new StringBuilder("[");
			Node<E> current = root, previous;
			while (true) {
				sb.append(current.data);
				sb.append(", ");
				if (current.next == null) {
					break;
				}
				previous = current;
				current = current.next;
			}
			sb.delete(sb.length()-2, sb.length());
			sb.append("] ");
                        sb.append(tail);
			return sb.toString();
		}
	}

	public MIterator<E> iterator() {
		return new MIterator<E>(root);
	}

	private class Node<E> {
		E data;
		Node<E> next;

		Node(E element) {
			data = element;
			next = null;
		}

		public String toString() {
			return data.toString();
		}

		Node(E element, Node<E> rest) {
			data = element;
			next = rest;
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

	protected class MIterator<E> implements Iterator<E> {
		Node<E> current, previous;

		private MIterator(Node<E> root) {
			current = root;
		}

		public boolean hasNext() {
			return current != null;
		}

		public E next() {
			if (current == null) {
				throw new NoSuchElementException();
			}
			Node<E> previous = current;
			current = current.next;
			return previous.data;
		}

		public void remove() {
			throw new UnsupportedOperationException();
		}

		public void set(E element) {
			current.data = element;
		}
	}
}
