class HashMap {
    private numItems = 0
    private capacity = 10000
    private keys[] = new int[self.capacity]
    private values[] = new int[self.capacity]
    private placed[] = new int[self.capacity]

    init = fun () -> (int) {
        libi = 0
        while (libi < self.capacity) {
            self.placed[libi] = 0
            libi = libi + 1
        }
    }

    put = fun (int, int) -> (int) {
        libhash = it0 % self.capacity

        while (self.placed[libhash] && (self.keys[libhash] != it0)) {
            libhash = (libhash + 1) % self.capacity
        }

        self.placed[libhash] = 1

        self.keys[libhash] = it0
        self.values[libhash] = it1

        return 0
    }

    get = fun (int) -> (int) {
        libhash = it0 % self.capacity

        while (self.placed[libhash] && (self.keys[libhash] != it0)) {
            libhash = (libhash + 1) % self.capacity
        }

        if (self.placed[libhash] == 0) {
            return 0
        }

        return self.values[libhash]
    }
}