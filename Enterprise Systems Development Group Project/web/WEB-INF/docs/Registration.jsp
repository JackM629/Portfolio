<%-- 
    Document   : Registration
    Created on : 27-Nov-2017, 13:16:36
    Author     : Frazer, Sasha
--%>
 <%@ include file="/resources/GoogleAddressLookUp.jsp" %>
<div class="main">
    <div class="content-screen">
        <div class="title">
            <h1>Registration</h1>
        </div>
        <form class="form" name="login" action="${pageContext.request.contextPath}/Registration" method="post">
            <div class="input-label">
                <label>First Name</label>
            </div>
            <input class="control-group" type="text" name="firstName"  value="">
            <div class="input-label">
                <label>Last Name</label>
            </div>
            <input class="control-group" type="text" name="lastName"  value="">
            <div class="input-label">
                <label>Date of Birth</label>
            </div>
            <input class="control-group" type="date" name="DOB"  value="">
            <div class="input-label">
                <label>Enter Your Full Address</label>
            </div>
            <input id="autocomplete"  placeholder="" onFocus="geolocate()" type="text">
            <div class="input-label">
                <label>No. &nbsp; &nbsp; &nbsp; &nbsp; Street Name</label>
            </div>
            <input class="houseNumber" name="houseNumber" id="street_number">
            <input class="streetname" name="streetName" id="route">
            <div class="input-label">
                <label>City</label>
            </div>
            <input class="control-group" name="city" id="locality">
            <div class="input-label">
                <label>Postcode</label>
            </div>
            <input class="control-group" name="postcode" id="postal_code">
            <div class="input-label">
                <label>Country</label>
            </div>
            <input class="control-group" name="country" id="country">
            <input class="btn" type="submit" value="Submit">
            <input class="btn" type="reset" value="Reset">
            <!--Dispaly the error message-->
            <c:if test="${not empty post}">
                ${ErrorMessageForRegistration}
            </c:if>
        </form>
    </div>
</div>