// could be optimized
struct Hash {
    float start;
    float freq_ratio;
    float time_delta;
    size_t id;

private:
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & start;
        ar & freq_ratio;
        ar & time_delta;
        ar & id;
    }
}