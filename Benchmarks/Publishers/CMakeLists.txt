add_custom_target(BenchmarkPublishers SOURCES
    BenchmarkResult.h
	BenchmarkPublisher.h
    StdoutBenchmarkPublisher.h
    MarkdownBenchmarkPublisher.h
)

set_target_properties(BenchmarkPublishers PROPERTIES FOLDER "Benchmarks")