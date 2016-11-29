public class ThreeHeap extends BinaryHeap {
	int[] children(int s) {
		if (s * 3 + 1 <= size()) {
			return new int[]{s*3 + 1, s*3, s*3 - 1};
		} else if (s * 3 <= size()) {
			return new int[]{s*3, s*3 - 1};
		} else if (s*3 - 1 <= size()) {
			return new int[]{s*3 - 1};
		} else {
			return new int[]{};
		}
	}

	int parent(int s) {
		if (s == 1) {
			return -1;
		} else {
			return (s + 1) / 3;
		}
	}
}
