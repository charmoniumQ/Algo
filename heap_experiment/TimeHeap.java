class TimeHeap {
	public static void main(String[] args) {
		BinaryHeap pq1 = new BinaryHeap();
		ThreeHeap pq2 = new ThreeHeap();
		MyPQ pq3 = new MyPQ();

		PriorityQueue pq = null;
		if (args[0].equals("1")) {
			pq = pq1;
		}
		if (args[0].equals("2")) {
			pq = pq2;
		}
		if (args[0].equals("3")) {
			pq = pq3;
		}
		if (pq == null) {
			System.out.println("Usage");
			System.exit(1);
		}

		System.out.println("iterations,insert time (ms),delete time (ms),delete phase1 (ms),delete phase2 (ms),delete phase3 (ms)");
		for (int i = 0; i < 20; ++i) {
			test(pq, i * 1000000L);
		}
	}

	public static void test(PriorityQueue pq, long n) {
		long start = System.nanoTime();

		for (long i = 0; i < n; ++i) {
			pq.insert(Math.random());
		}
		long insertTime = (System.nanoTime() - start) / 1000000L;

		start = System.nanoTime();
		for (long i = 0; i < n; ++i) {
			pq.deleteMin();
		}
		long deleteTime = (System.nanoTime() - start) / 1000000L;

		System.out.println(n + "," + insertTime + "," + deleteTime);
		//System.out.println(n + "," + insertTime + "," + deleteTime + "," + pq.t1 + "," + pq.t2 + "," + pq.t3);
		//pq.t1 = 0;
		//pq.t2 = 0;
		//pq.t3 = 0;
	}
}
