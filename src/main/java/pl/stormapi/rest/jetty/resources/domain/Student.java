package pl.stormapi.rest.jetty.resources.domain;

public class Student {

    private String name;
    private Long indexNo;

    public Student() {

    }

    public Student(final String name, final Long indexNo) {
        super();
        this.name = name;
        this.indexNo = indexNo;
    }

    public String getName() {
        return name;
    }

    public void setName(final String name) {
        this.name = name;
    }

    public Long getIndexNo() {
        return indexNo;
    }

    public void setIndexNo(final Long indexNo) {
        this.indexNo = indexNo;
    }

}
