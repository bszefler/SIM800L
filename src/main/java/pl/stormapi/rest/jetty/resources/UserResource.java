package pl.stormapi.rest.jetty.resources;

import java.util.Random;

import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

import pl.stormapi.rest.jetty.resources.domain.Student;

@Path("/user")
public class UserResource {

    @GET
    @Produces(MediaType.APPLICATION_JSON)
    public Response user(@QueryParam("name") final String name) {
        final Student s = new Student(name + " test222", new Random().nextLong());

        return Response.ok().entity(s).build();
        // return new Student(name, new Random().nextLong());
    }

}
